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

struct TankBattleCommand
{
    int playerID = -1;                  // if left -1, server will issue an ID
                                        // upon reciept of a message

    tankBattleMessage msg;              // declare message type
    TankMovementOptions tankMove;       // assign an action for the tank
    CannonMovementOptions cannonMove;   // TODO: implement rotate cannon on tank gun

    int fireWish = 0;
    int messageLength;
};

struct TankBattleStateData
{
    int playerID;

    float position[3];
    float forward[3];

    float cannonForward[3];

    bool canFire;
    bool enemyInSight;
};