#pragma once
#include "TankBattleHeaders.h"
#include "matth.h"

class Agent {
	struct LastKnowLocation {
		matth::vec2 pos;
		float timePassed;
	};

	enum CannonState {
		CANNON_INIT, SMART_SCAN, ROTATE_SCAN, AIM, FIRE
	};

	enum TankState {
		TANK_INIT, WANDER, SMART_SCAN_MOVE
	};

public:
	Agent();
	tankNet::TankBattleCommand update( const tankNet::TankBattleStateData& stateData, float deltaTime );
private:
	CannonState cannonState;
	TankState tankState;
	bool firstFrame;
	tankNet::TankBattleStateData prevState, currState;
	tankNet::TankBattleCommand command;
	float dt;
	LastKnowLocation knowLocations[4];

	// tank attributes
	float tankSpeed;
	float cannonRotateSpeed;
	float tankRotateSpeed;
	matth::vec2 mapMin, mapMax;
	float fieldOfView;

	/////////////////////// CANNON LOGIC VARIABLES /////////////////////////////
	// init variables
	float cannonRotateTestTime;
	// rotate scan variables
	tankNet::CannonMovementOptions scanRotateDir;
	// smart scan veriables
	int smartScanTarget;
	float reasonableRadius;
	// aim variables
	int targetPlayer;

	/////////////////////// TANK BASE LOGIC VARIABLES //////////////////////////
	// init variables
	float tankSpeedTestTime;
	// wander variables
	const int WANDER_MIN, WANDER_MAX;
	float wanderDt;
	matth::vec2 wanderTarget;
	float distToTargetSwitch;

	// cannon functions
	void cannonInit();
	void smartScan();
	void rotateScan();
	void aim();
	void fire();

	// tank base functions
	void wander();
	void tankInit();
	void smartScanMove();

	// util functions
	float GetRandomFloat( float low, float high ) const;
	int GetBestAlignedEnemyInSight() const;
	void moveTo(matth::vec2 targetPos);
};