using UnityEngine;
using System.Collections;

public enum TankBattleMessage
{
    NONE,
    JOIN,
    GAME,
    QUIT
}

public enum TankMovementOptions
{
    HALT,
    FWRD,
    BACK,
    LEFT,
    RIGHT
}

public enum CannonMovementOptions
{
    HALT,
    LEFT,
    RIGHT
}

public struct TankBattleHeader
{
    public int uuid;
    public int playerID;


    public TankBattleMessage msg;
    public TankMovementOptions tankMove;
    public CannonMovementOptions cannonMove;

    public int fireWish;

    public int messageLength;
}
