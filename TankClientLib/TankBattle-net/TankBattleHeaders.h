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

	struct TankBattleHeader
	{
		TankBattleMessage msg;              // declare message type
		TankMovementOptions tankMove;       // assign an action for the tank
		CannonMovementOptions cannonMove;   // TODO: implement rotate cannon on tank gun

		int fireWish = 0;
		int messageLength = sizeof(TankBattleHeader);
	};

	struct TankTacticoolInfo
	{
		int playerID;

		int inSight;
		float lastKnownPosition[3];
		float lastKnownDirection[3];
	};

	struct TankBattleStateData
	{
		int playerID;
		float currentHealth;

		float position[3];
		float forward[3];

		float cannonForward[3];

		int canFire;
		int tacticoolCount;

		TankTacticoolInfo tacticoolTest;
		TankTacticoolInfo * tacticoolData;

		enum OFFSETS
		{
			PLAYER_ID		= 0,
			POSITION		= PLAYER_ID + sizeof(int),
			FORWARD			= POSITION + sizeof(float) * 3,
			CANNON_FORWARD	= FORWARD + sizeof(float) * 3,
			CAN_FIRE		= CANNON_FORWARD + sizeof(float) * 3,
			TACTICOOL_COUNT = CAN_FIRE + sizeof(int),
			TACTICOOL_ARRAY = TACTICOOL_COUNT + sizeof(int),
			END				= TACTICOOL_ARRAY + sizeof(TankTacticoolInfo *)
		};
	};
}