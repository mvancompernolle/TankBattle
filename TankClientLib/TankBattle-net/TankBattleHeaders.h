#pragma once

enum class tankBattleMessage
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

struct tankBattleHeader
{
    tankBattleMessage msg;              // declare message type
    TankMovementOptions tankMove;       // assign an action for the tank
    CannonMovementOptions cannonMove;   // TODO: implement rotate cannon on tank gun

    int fireWish = 0;
    int messageLength = sizeof(tankBattleHeader);
};

struct TankTacticoolInfo
{
    int playerID;

    bool inSight;
    float lastKnownPosition[3];
    float lastKnownDirection[3];
};

struct TankBattleStateData
{
    int playerID;

    float position[3];
    float forward[3];

    float cannonForward[3];

    bool canFire;
    int tacticoolCount;
    TankTacticoolInfo * tacticoolData;
};