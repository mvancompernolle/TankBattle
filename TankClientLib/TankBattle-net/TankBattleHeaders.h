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
    int playerID = -1;                  // if left -1, server will issue an ID
                                        // upon reciept of a message

    tankBattleMessage msg;              // declare message type
    TankMovementOptions tankMove;       // assign an action for the tank
    CannonMovementOptions cannonMove;   // TODO: implement rotate cannon on tank gun

    int fireWish = 0;
    int messageLength = sizeof(tankBattleHeader);
};

struct TankBattleStateData
{
    int playerID;

    float position[3];
    float forward[3];

    float cannonForward[3];

    bool canFire;
    bool enemyInSight;

    float lastKnownPosition[3];   // last known enemy position (must have LOS or fire)
    float lastKnownDirection[3];  // must be in proximity and moving
};