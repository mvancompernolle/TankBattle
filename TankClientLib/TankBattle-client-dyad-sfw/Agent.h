#pragma once
#include "TankBattleHeaders.h"

class Agent {
	enum CannonState {
		SCAN, AIM, FIRE
	};

	enum TankState {
		WANDER
	};

public:
	Agent();
	tankNet::TankBattleCommand update( const tankNet::TankBattleStateData& stateData );
private:
	CannonState cannonState;
	TankState tankState;
	tankNet::TankBattleStateData prevousState;

	// cannon functions
	void scan();
	void aim();
	void fire();

	// tank base functions
};