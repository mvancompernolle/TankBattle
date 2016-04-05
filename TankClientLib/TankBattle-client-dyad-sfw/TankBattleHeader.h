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
    int uuid = -1;
    int playerID = -1;

    tankBattleMessage msg = tankBattleMessage::NONE;
    TankMovementOptions tankMove = TankMovementOptions::HALT;
    CannonMovementOptions cannonMove = CannonMovementOptions::HALT;

    int fireWish = 0;

    int messageLength;  // this is only if we had like, a payload to deliver.
};