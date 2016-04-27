#pragma once
#include "Agent.h"
#include <ctime>
#include <cstdlib>
#include <string>

Agent::Agent() : firstFrame( true ), tankSpeedTestTime( 0.5f ), cannonRotateTestTime( 0.5f ),
	tankState( TankState::WANDER ), cannonState( CannonState::ROTATE_SCAN ), tankSpeed( 12.0f ), cannonRotateSpeed( 90.0f ),
	tankRotateSpeed( 180.0f ), mapMin{ -50.0f, -50.0f }, mapMax{ 50.0f, 50.0f }, WANDER_MIN( 4 ), WANDER_MAX( 6 ), wanderDt( 0.0f ),
	fieldOfView( 90.0f ), distToTargetSwitch( 10.0f ), reasonableRadius(mapMax.x / 2.0f), explosionRadius(5.0f), fireCooldown(0.5f),
	visionDist(50.0f), fireDist(18.0f)
{
	srand( time( nullptr ) );
	scanRotateDir = rand() % 2 == 0 ? tankNet::CannonMovementOptions::RIGHT : tankNet::CannonMovementOptions::LEFT;
	wanderDt = GetRandomFloat( WANDER_MIN, WANDER_MAX );
	wanderTarget = { GetRandomFloat( mapMin.x, mapMax.x ), GetRandomFloat( mapMin.y, mapMax.y ) };

	for each( auto& loc in knownLocations ) {
		loc.pos = {};
		loc.timePassed = INFINITY;
	}
}

tankNet::TankBattleCommand Agent::update( const tankNet::TankBattleStateData& stateData, float deltaTime ) {
	dt = deltaTime;
	prevState = currState;
	currState = stateData;

	// default tank command
	command.msg = tankNet::TankBattleMessage::NONE;
	command.tankMove = tankNet::TankMovementOptions::HALT;
	command.cannonMove = tankNet::CannonMovementOptions::HALT;
	command.fireWish = false;

	// update last known player location and times
	for ( int i = 0; i < currState.tacticoolCount; ++i ) {
		const auto& enemyData = currState.tacticoolData[i];
		knownLocations[i].pos = {enemyData.lastKnownPosition[0], enemyData.lastKnownPosition[2]};
		if ( knownLocations[i].pos != matth::vec2{prevState.tacticoolData[i].lastKnownPosition[0], prevState.tacticoolData[i].lastKnownPosition[2] }
			|| enemyData.inSight ) {
			knownLocations[i].timePassed = 0.0f;
		}
		else {
			knownLocations[i].timePassed += dt;
		}
	}

	// update agent state
	std::string cannonStr, tankStr;
	switch ( cannonState ) {
	case CannonState::SMART_SCAN: smartScan(); cannonStr =  "smart scan"; break;
	case CannonState::ROTATE_SCAN: rotateScan(); cannonStr = "rotate scan"; break;
	case CannonState::AIM: aim(); cannonStr = "aim"; break;
	case CannonState::FIRE: fire(); cannonStr = "fire"; break;
	}
	switch ( tankState ) {
	case TankState::WANDER: wander(); tankStr = "wander"; break;
	case TankState::SMART_SCAN_MOVE: smartScanMove(); tankStr = "smart move"; break;
	case TankState::HOVER: hover(); tankStr = "hover"; break;
	}
	std::cout << cannonStr << " - " << tankStr << std::endl;

	firstFrame = false;

	return command;
}

void Agent::smartScan() {
	int playerInSight = GetBestAlignedEnemyInSight();

	// no enemy is in sight
	if ( playerInSight == -1 ) {

		// if enemy positions was updated a reasonable amount of time ago, search for them
		float maxOffsetFromLastSighting = knownLocations[smartScanTarget].timePassed * tankSpeed * 0.75f;
		maxOffsetFromLastSighting = maxOffsetFromLastSighting < 5.0f ? 5.0f : maxOffsetFromLastSighting;

		if ( maxOffsetFromLastSighting > reasonableRadius ) {
			smartScanTarget = GetSmartTarget();
			if ( smartScanTarget == -1 ) {
				scanRotateDir = rand() % 2 == 0 ? tankNet::CannonMovementOptions::RIGHT : tankNet::CannonMovementOptions::LEFT;
				cannonState = CannonState::ROTATE_SCAN;
				tankState = TankState::WANDER;
				return;
			}
		}

		// determine the worst case points the player could be at and turn until it is reach in fov
		const auto& enemyData = currState.tacticoolData[smartScanTarget];
		const matth::vec2 myPos = matth::vec2{ currState.position[0], currState.position[2] };
		const matth::vec2 targetDir = knownLocations[smartScanTarget].pos - myPos;
		const matth::vec2 cannonForward = { currState.cannonForward[0], currState.cannonForward[2] };
		const matth::vec2 perpRightOffset = -( targetDir.perp() * maxOffsetFromLastSighting );

		if ( scanRotateDir == tankNet::CannonMovementOptions::RIGHT ) {
			const matth::vec2 mostRight = ((knownLocations[smartScanTarget].pos + perpRightOffset) - myPos).normal();
			//std::cout << "right dist: " << ( knownLocations[smartScanTarget].pos - mostRight ).length() << std::endl;
			// if right scan reaches end point, start scanning left
			if ( matth::dot(mostRight, cannonForward) >= cos(45.0f * DEGTORAD) || matth::dot(-cannonForward.perp(), mostRight) <= 0.0f) {
				scanRotateDir = tankNet::CannonMovementOptions::LEFT;
				command.cannonMove = tankNet::CannonMovementOptions::LEFT;
			}
			else {
				command.cannonMove = tankNet::CannonMovementOptions::RIGHT;
			}
		}
		else {
			const matth::vec2 mostLeft = ((knownLocations[smartScanTarget].pos - perpRightOffset) - myPos).normal();
			// if left scan reaches end point, start scanning right
			if ( matth::dot( mostLeft, cannonForward ) >= cos( 45.0f * DEGTORAD ) || matth::dot( cannonForward.perp(), mostLeft.normal() ) <= 0.0f ) {
				scanRotateDir = tankNet::CannonMovementOptions::RIGHT;
				command.cannonMove = tankNet::CannonMovementOptions::RIGHT;
			}
			else {
				command.cannonMove = tankNet::CannonMovementOptions::LEFT;
			}
		}
	}
	else {
		// if player is found aim at them
		targetPlayer = playerInSight;
		cannonState = CannonState::AIM;
		tankState = TankState::WANDER;
	}
}

void Agent::smartScanMove() {
	// get direction to the pos the target player was seen
	const auto& enemyData = currState.tacticoolData[smartScanTarget];
	const matth::vec2 targetOffset = knownLocations[smartScanTarget].pos - matth::vec2{ currState.position[0], currState.position[2] };
	const float maxOffsetFromLastSighting = knownLocations[smartScanTarget].timePassed * tankSpeed * 0.75f;

	if ( wanderTarget.x > mapMin.x && wanderTarget.x < mapMax.x && wanderTarget.y > mapMin.y && wanderTarget.y < mapMax.y ) {
		moveTo( knownLocations[smartScanTarget].pos - ( targetOffset.normal() * maxOffsetFromLastSighting ) );
	} else {
		tankState = TankState::WANDER;
	}
}

void Agent::rotateScan() {
	int playerInSight = GetBestAlignedEnemyInSight();

	// if an enemy was found, target them
	if ( playerInSight != -1 ) {
		// change state to aiming
		targetPlayer = playerInSight;
		cannonState = CannonState::AIM;
		return;
	}
	
	smartScanTarget = GetSmartTarget();
	if ( GetSmartTarget() != -1 ) {
		scanRotateDir = rand() % 2 == 0 ? tankNet::CannonMovementOptions::RIGHT : tankNet::CannonMovementOptions::LEFT;
		cannonState = CannonState::SMART_SCAN;
		tankState = TankState::SMART_SCAN_MOVE;
		return;
	}

	// rotate the turret in circles until enemy is found
	command.cannonMove = scanRotateDir;
}

void Agent::aim() {
	// get the last know direction of the enemy tank
	const auto& enemyData = currState.tacticoolData[targetPlayer];

	// if target is no longer in line of sight, start scanning
	if ( !enemyData.inSight ) {
		scanRotateDir = rand() % 2 == 0 ? tankNet::CannonMovementOptions::RIGHT : tankNet::CannonMovementOptions::LEFT;
		smartScanTarget = targetPlayer;
		// pick scan direction based on direction of enemy's last know location
		cannonState = CannonState::SMART_SCAN;
		tankState = TankState::SMART_SCAN_MOVE;
		return;
	}
	else {
		matth::vec2 myPos = matth::vec2{ currState.position[0], currState.position[2] };
		matth::vec2 targetPos = { enemyData.lastKnownPosition[0], enemyData.lastKnownPosition[2] };

		matth::vec2 targetForward = targetPos - matth::vec2{ prevState.tacticoolData[targetPlayer].lastKnownPosition[0], prevState.tacticoolData[targetPlayer].lastKnownPosition[2] };
		matth::vec2 targetDir;
		if ( targetForward != matth::vec2{0.0f, 0.0f} ) {
			matth::vec2 predictedPos = targetPos + targetForward * dt;
			matth::vec2 bulletLead = targetForward.normal() * ( targetPos - myPos ).length() * 0.5f;
			targetDir = ( ( predictedPos + bulletLead ) - myPos ).normal();
		}
		else {
			targetDir = targetPos - myPos;
		}

		matth::vec2 cannonDir = matth::vec2{ currState.cannonForward[0], currState.cannonForward[2] };
		matth::vec2 myCannonRightVec = -cannonDir.perp();
		const float rightDot = matth::dot( myCannonRightVec, targetDir );

		// aim at the target ank if still in sight
		//std::cout << "forward dot: " << matth::dot( cannonDir, targetDir ) << " right dot : " << rightDot << std::endl;
		if ( matth::dot( cannonDir, targetDir ) < 0.98f ) {
			if ( rightDot >= 0.0f ) {
				command.cannonMove = tankNet::CannonMovementOptions::RIGHT;
			}
			else {
				command.cannonMove = tankNet::CannonMovementOptions::LEFT;
			}
		}
		else if ( currState.canFire ) {
			cannonState = CannonState::FIRE;
		}
	}
}

void Agent::fire() {
	command.fireWish = true;
	cannonState = CannonState::AIM;
}

void Agent::wander() {
	// move to a random direction on the map
	wanderDt -= dt;

	// check if need to pick a new wander point
	const matth::vec2 currPos = matth::vec2{ currState.position[0], currState.position[2] };
	const matth::vec2 currForward = matth::vec2{ currState.forward[0], currState.forward[2] };
	if ( matth::vec2{ wanderTarget - currPos }.length() < distToTargetSwitch || wanderDt <= 0.0f) {
		wanderDt = GetRandomFloat( WANDER_MIN, WANDER_MAX );
		wanderTarget = { GetRandomFloat( mapMin.x, mapMax.x ), GetRandomFloat( mapMin.y, mapMax.y ) };
	}

	moveTo( wanderTarget );
}

void Agent::hover() {

}

float Agent::GetRandomFloat(float low, float high) const {
	return low + static_cast <float> ( rand() ) / ( static_cast <float> ( RAND_MAX / ( high - low ) ) );
}

int Agent::GetBestAlignedEnemyInSight() const {
	int bestTarget = -1;
	float bestAligned = 0.0f;
	for ( int i = 0; i < currState.tacticoolCount; ++i ) {
		// check if enemy is in line of sight
		const auto& enemyData = currState.tacticoolData[i];
		if ( enemyData.inSight ) {
			const matth::vec2 currPos = matth::vec2{ currState.position[0], currState.position[2] };
			const matth::vec2 cannonForwardDir = matth::vec2{ currState.forward[0], currState.forward[2] };
			const matth::vec2 targetDir = matth::vec2{ wanderTarget - currPos }.normal();
			// get enemy that is closest to current cannon direction
			const float dotVal = matth::dot( cannonForwardDir, targetDir );
			if ( dotVal > bestAligned ) {
				bestAligned = dotVal;
				bestTarget = i;
			}
		}
	}
	return bestTarget;
}

void Agent::moveTo( matth::vec2 targetPos ) {
	// move forward/backward if almost aligned with target direction
	const matth::vec2 currPos = matth::vec2{ currState.position[0], currState.position[2] };
	const matth::vec2 currForward = matth::vec2{ currState.forward[0], currState.forward[2] };
	const matth::vec2 targetOffset = matth::vec2{ targetPos - currPos };
	const float alignDot = matth::dot( targetOffset.normal(), currForward );
	const matth::vec2 tankRightVec = -currForward.perp();
	float rightDot = matth::dot( tankRightVec, targetOffset.normal() );

	if ( alignDot >= 0.0f ) {
		if ( alignDot >= 0.87f ) {
			command.tankMove = tankNet::TankMovementOptions::FWRD;
		} else {
			command.tankMove = rightDot >= 0.0f ? tankNet::TankMovementOptions::RIGHT : tankNet::TankMovementOptions::LEFT;
		}
	} else {
		if ( alignDot <= -0.87f ) {
			command.tankMove = tankNet::TankMovementOptions::BACK;
		} else {
			command.tankMove = rightDot >= 0.0f ? tankNet::TankMovementOptions::LEFT : tankNet::TankMovementOptions::RIGHT;
		}
	}
}

int Agent::GetSmartTarget() const {
	int bestTarget = -1;
	float bestDistance = INFINITY;
	for ( int i = 0; i < 4; ++i ) {
		const auto& loc = knownLocations[i];
		float maxOffsetFromLastSighting = loc.timePassed * tankSpeed * 0.75f;
		if ( maxOffsetFromLastSighting < reasonableRadius ) {
			const float distToRadius = (loc.pos - matth::vec2{ currState.position[0], currState.position[2] }).length() - maxOffsetFromLastSighting;
			if ( distToRadius < bestDistance ) {
				bestDistance = distToRadius;
				bestTarget = i;
			}
		}
	}
	return bestTarget;
}