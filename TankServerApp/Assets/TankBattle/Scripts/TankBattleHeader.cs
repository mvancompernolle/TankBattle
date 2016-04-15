using UnityEngine;

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
    public TankBattleMessage msg;               // declare message type
    public TankMovementOptions tankMove;        // assign action for the tank treads
    public CannonMovementOptions cannonMove;    // TODO: implement rotation on tank gun

    public int fireWish;
    public int messageLength;
}

public struct TankTacticoolInfo
{
    public int playerID;

    public int inSight;
    public Vector3 lastKnownPosition;
    public Vector3 lastKnownDirection;
}

public struct TankBattleStateData
{
    public int playerID;
    public float currentHealth;

    public Vector3 position;
    public Vector3 forward;

    public Vector3 cannonForward;

    public int canFire;

    public int perceptCount; 
}