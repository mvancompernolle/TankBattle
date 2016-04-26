#pragma once
#include "Agent.h"
#include <ctime>
#include <cstdlib>

Agent::Agent() : firstFrame( true ), tankSpeedTestTime( 0.5f ), cannonRotateTestTime( 0.5f ),
	tankState( TankState::WANDER ), cannonState( CannonState::ROTATE_SCAN ), tankSpeed( 12.0f ), cannonRotateSpeed( 90.0f ),
	tankRotateSpeed( 180.0f ), mapMin{ -50.0f, -50.0f }, mapMax{ 50.0f, 50.0f }, WANDER_MIN( 4 ), WANDER_MAX( 6 ), wanderDt( 0.0f ),
	fieldOfView( 90.0f ), distToTargetSwitch( 10.0f ), reasonableRadius(mapMax.x)
{
	srand( time( nullptr ) );
	scanRotateDir = rand() % 2 == 0 ? tankNet::CannonMovementOptions::RIGHT : tankNet::CannonMovementOptions::LEFT;
	wanderDt = GetRandomFloat( WANDER_MIN, WANDER_MAX );
	wanderTarget = { GetRandomFloat( mapMin.x, mapMax.x ), GetRandomFloat( mapMin.y, mapMax.y ) };
}

tankNet::TankBattleCommand Agent::update( const tankNet::TankBattleStateData& stateData, float deltaTime ) {
	dt = deltaTime;
	prevState = currState;
	currState = stateData;

	// default tank command
	command.msg = tankNet::TankBattleMessage::NONE;
	command.tankMove = tankNet::TankMovementOptions::HALT;
	command.cannonMove = tankNet::CannonMovementOptions::HALT;

	// update last known player location and times
	for ( int i = 0; i < currState.tacticoolCount; ++i ) {
		const auto& enemyData = currState.tacticoolData[i];
		knowLocations[i].pos = {enemyData.lastKnownPosition[0], enemyData.lastKnownPosition[2]};
		if ( enemyData.inSight ) {
			knowLocations[i].timePassed = 0.0f;
		}
		else {
			knowLocations[i].timePassed += dt;
		}
	}

	// update agent state
	switch ( cannonState ) {
	case CannonState::CANNON_INIT: cannonInit(); break;
	case CannonState::SMART_SCAN: smartScan(); break;
	case CannonState::ROTATE_SCAN: rotateScan(); break;
	case CannonState::AIM: aim(); break;
	case CannonState::FIRE: fire(); break;
	}
	switch ( tankState ) {
	case TankState::TANK_INIT: tankInit(); break;
	case TankState::WANDER: wander(); break;
	}

	firstFrame = false;

	return command;
}

void Agent::smartScan() {
	int playerInSight = GetBestAlignedEnemyInSight();
	// no enemy is in sight
	if ( playerInSight == -1 ) {
		// if enemy positions was updated a reasonable amount of time ago, search for them
		if ( (knowLocations[smartScanTarget].timePassed * tankSpeed * dt * 0.75f) < reasonableRadius ) {
			// determine the worst can points the player could be at
			const auto& enemyData = currState.tacticoolData[smartScanTarget];
			const matth::vec2 targetDir = { enemyData.lastKnownDirection[0], enemyData.lastKnownDirection[2] };
			const matth::vec2 mostRight = knowLocations[smartScanTarget].pos - targetDir.perp();
			const matth::vec2 mostLeft = knowLocations[smartScanTarget].pos + targetDir.perp();
		}
		else {
			scanRotateDir = rand() % 2 == 0 ? tankNet::CannonMovementOptions::RIGHT : tankNet::CannonMovementOptions::LEFT;
			cannonState = CannonState::ROTATE_SCAN;
		}
	}
	else {

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

	// rotate the turret in circles until enemy is found
	command.cannonMove = scanRotateDir;
}

void Agent::aim() {
	const auto& enemyData = currState.tacticoolData[targetPlayer];
	// if target is no longer in line of sight, start scanning
	if ( !enemyData.inSight ) {
		scanRotateDir = rand() % 2 == 0 ? tankNet::CannonMovementOptions::RIGHT : tankNet::CannonMovementOptions::LEFT;
		smartScanTarget = targetPlayer;
		cannonState = CannonState::SMART_SCAN;
		return;
	}

	// get the last know direction of the enemy tank
	matth::vec2 targetPos = { enemyData.lastKnownPosition[0], enemyData.lastKnownPosition[2] };
	matth::vec2 targetDir = { enemyData.lastKnownDirection[0], enemyData.lastKnownDirection[2] };
	matth::vec2 targetForward = { enemyData.lastKnownTankForward[0], enemyData.lastKnownTankForward[2] };
	
	matth::vec2 myCannonRightVec = matth::vec2{ currState.cannonForward[0], currState.cannonForward[2] }.perp();
	float rightDot = matth::dot(myCannonRightVec, targetDir);
	command.cannonMove = rightDot <= 0.0f ? tankNet::CannonMovementOptions::RIGHT : tankNet::CannonMovementOptions::LEFT;
}

void Agent::fire() {

}

void Agent::wander() {
	// move to a random direction on the map
	wanderDt -= dt;

	// check if need to pick a new wander point
	const matth::vec2 currPos = matth::vec2{ currState.position[0], currState.position[2] };
	const matth::vec2 currForward = matth::vec2{ currState.forward[0], currState.forward[2] };
	if ( matth::vec2{ wanderTarget - currPos }.length() < distToTargetSwitch || wanderDt <= 0.0f) {
		std::cout << "new target" << std::endl;
		wanderDt = GetRandomFloat( WANDER_MIN, WANDER_MAX );
		wanderTarget = { GetRandomFloat( mapMin.x, mapMax.x ), GetRandomFloat( mapMin.y, mapMax.y ) };
	}

	// move forward/backward if almost aligned with target direction
	const matth::vec2 targetOffset = matth::vec2{ wanderTarget - currPos };
	const float alignDot = matth::dot( targetOffset.normal(), currForward );
	const matth::vec2 tankRightVec = -currForward.perp();
	float rightDot = matth::dot( tankRightVec, targetOffset.normal() );
	
	if ( alignDot >= 0.87f ) {
		command.tankMove = tankNet::TankMovementOptions::FWRD;
	}
	else if ( alignDot <= -0.87f ) {
		command.tankMove = tankNet::TankMovementOptions::BACK;
	}
	else {
		command.tankMove = tankNet::TankMovementOptions::RIGHT;
	}

	/*
	//std::cout << "align: " << alignDot << " right: " << rightDot << std::endl;
	if ( alignDot >= 0.0f) {
		//std::cout << "forward" << std::endl;
		if ( alignDot >= 0.87f ) {
			command.tankMove = tankNet::TankMovementOptions::FWRD;
		}
		else {
			command.tankMove = rightDot >= 0.0f ? tankNet::TankMovementOptions::RIGHT : tankNet::TankMovementOptions::LEFT;
		}
	}
	else{
		//std::cout << "backward" << std::endl;
		if ( alignDot <= -0.87f ) {
			command.tankMove = tankNet::TankMovementOptions::BACK;
		} else {
			command.tankMove = rightDot >= 0.0f ? tankNet::TankMovementOptions::LEFT : tankNet::TankMovementOptions::RIGHT;
		}
	}
	*/
}

void Agent::cannonInit() {
	// turn cannon one second to determine turn speed
	if ( !firstFrame && cannonRotateTestTime > 0.0f ) {
		command.cannonMove = tankNet::CannonMovementOptions::RIGHT;
		matth::vec2 oldDir = matth::vec2{ prevState.cannonForward[0], prevState.cannonForward[2] };
		matth::vec2 currDir = matth::vec2{ currState.cannonForward[0], currState.cannonForward[2] };
		const float rotSpeed = (std::cos( matth::dot( oldDir, currDir ) / ( oldDir.length() * currDir.length() ) ) * RADTODEG) / dt;
		cannonRotateSpeed = matth::max( rotSpeed, cannonRotateSpeed );
		cannonRotateTestTime -= dt;
	} else if ( cannonRotateTestTime <= 0.0f ) {
		cannonState = CannonState::ROTATE_SCAN;
	}
}

void Agent::tankInit() {
	// move forward for one second to determine tank speed
	if ( !firstFrame && tankSpeedTestTime > 0.0f ) {
		command.tankMove = tankNet::TankMovementOptions::FWRD;
		matth::vec2 offset = matth::vec2{ currState.position[0], currState.position[2] } -
							matth::vec2{ prevState.position[0], prevState.position[2] };
		tankSpeed = matth::max( offset.length() / dt, tankSpeed );
		tankSpeedTestTime -= dt;
	}
	else if ( tankSpeedTestTime <= 0.0f ) {
		tankState = TankState::WANDER;
	}
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