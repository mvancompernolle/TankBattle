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
    typedef int BOOL;

    int playerID;

    BOOL inSight;
    float lastKnownPosition[3];
    float lastKnownDirection[3];
};

struct TankBattleStateData
{
    typedef int BOOL;

    int playerID;

    float position[3];
    float forward[3];

    float cannonForward[3];

    BOOL canFire;
    int tacticoolCount;
    TankTacticoolInfo * tacticoolData;

    enum OFFSETS
    {
        PLAYER_ID       = 0,
        POSITION        = PLAYER_ID         + sizeof(int),
        FORWARD         = POSITION          + sizeof(float) * 3,
        CANNON_FORWARD  = FORWARD           + sizeof(float) * 3,
        CAN_FIRE        = CANNON_FORWARD    + sizeof(float) * 3,
        TACTICOOL_COUNT = CAN_FIRE          + sizeof(BOOL),
        TACTICOOL_ARRAY = TACTICOOL_COUNT   + sizeof(int),
        END             = TACTICOOL_ARRAY   + sizeof(TankTacticoolInfo *)
    };
};

//struct TankBattleDummyData
//{
//                                // OFFSET
//    int playerID;               // 0
//
//    float position[3];          // 4
//    float forward[3];           // 16
//
//    float cannonForward[3];     // 28
//
//    BOOL canFire;               // 40
//    int tacticoolCount;         // 44
//
//                                // 48
//};


//enum testing
//{
//    INTE = sizeof(int),
//    FLOT = sizeof(float),
//    BOOL = sizeof(bool)
//};