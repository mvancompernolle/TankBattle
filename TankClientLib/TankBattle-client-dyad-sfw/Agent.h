#pragma once
#include "TankBattleHeaders.h"
#include "matth.h"

class Agent {
	struct LastKnowLocation {
		matth::vec2 pos;
		float timePassed;
	};

	enum CannonState {
		SMART_SCAN, ROTATE_SCAN, AIM, FIRE
	};

	enum TankState {
		WANDER, SMART_SCAN_MOVE, HOVER
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
	LastKnowLocation knownLocations[4];

	// tank attributes
	const float tankSpeed;
	const float cannonRotateSpeed;
	const float tankRotateSpeed;
	const matth::vec2 mapMin, mapMax;
	const float fieldOfView;
	const float fireCooldown;
	const float explosionRadius;
	const float visionDist;
	const float fireDist;

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
	void smartScan();
	void rotateScan();
	void aim();
	void fire();

	// tank base functions
	void wander();
	void smartScanMove();
	void hover();

	// util functions
	float GetRandomFloat( float low, float high ) const;
	int GetBestAlignedEnemyInSight() const;
	void moveTo(matth::vec2 targetPos);
	int GetSmartTarget() const;
};