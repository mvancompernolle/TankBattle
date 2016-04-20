#pragma once
#include "Agent.h"

Agent::Agent() {
	cannonState = CannonState::SCAN;
	tankState = TankState::WANDER;
}

tankNet::TankBattleCommand Agent::update( const tankNet::TankBattleStateData& stateData ) {
	tankNet::TankBattleCommand command;
	command.msg = tankNet::TankBattleMessage::NONE;
	command.tankMove = tankNet::TankMovementOptions::HALT;
	command.cannonMove = tankNet::CannonMovementOptions::HALT;

	// update agent state
	switch ( cannonState ) {
	case CannonState::SCAN:
		scan();
		break;
	case CannonState::AIM:
		aim();
		break;
	case CannonState::FIRE:
		fire();
		break;
	}
	switch ( tankState ) {
	case TankState::WANDER:
		command.tankMove = tankNet::TankMovementOptions::FWRD;
		break;
	}

	return command;
}

void Agent::scan() {

}

void Agent::aim() {

}

void Agent::fire() {

}