#pragma once

#include <ostream>

namespace tankNet
{
	enum class TankBattleMessage
	{
		NONE,
		JOIN,
		GAME,
		QUIT
	};

	enum class TankMovementOptions
	{
		HALT,
		FWRD,
		BACK,
		LEFT,
		RIGHT,
	};

	enum class CannonMovementOptions
	{
		HALT,
		LEFT,
		RIGHT
	};

	struct TankBattleCommand
	{
		TankBattleMessage msg;              // declare message type
		TankMovementOptions tankMove;       // assign an action for the tank
		CannonMovementOptions cannonMove;   // TODO: implement rotate cannon on tank gun

		bool fireWish = 0;
		int messageLength = sizeof(TankBattleCommand);
	};

	struct TankTacticoolInfo
	{
		int playerID;

		bool inSight;
		float lastKnownPosition[3];
		float lastKnownDirection[3];

		//float lastKnownTankForward[3];		// TODO: implement lastKnownTankForward
		//float lastKnownCannonForward[3];	// TOOD: implement lastKnownCannonForward
	};

	struct TankBattleStateData
	{
		int messageLength;

		int playerID;
		float currentHealth;

		float position[3];
		float forward[3];

		float cannonForward[3];

		bool canFire;
		int tacticoolCount;


		TankTacticoolInfo * tacticoolData;

		enum OFFSETS
		{
			MESSAGE_LENGTH  = 0,
			PLAYER_ID		= MESSAGE_LENGTH + sizeof(int),
			CURRENT_HEALTH  = PLAYER_ID + sizeof(int),
			POSITION		= CURRENT_HEALTH + sizeof(int),
			FORWARD			= POSITION + sizeof(float) * 3,
			CANNON_FORWARD	= FORWARD + sizeof(float) * 3,
			CAN_FIRE		= CANNON_FORWARD + sizeof(float) * 3,
			TACTICOOL_COUNT = CAN_FIRE + sizeof(bool),
			TACTICOOL_ARRAY = TACTICOOL_COUNT + sizeof(int),
		};
	};
}