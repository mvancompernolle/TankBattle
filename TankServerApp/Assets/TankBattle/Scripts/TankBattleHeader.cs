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
    public int playerID;                        // if left -1, server will issue an ID
                                                // upon reciept of a message

    public TankBattleMessage msg;               // declare message type
    public TankMovementOptions tankMove;        // assign action for the tank treads
    public CannonMovementOptions cannonMove;    // TODO: implement rotation on tank gun

    public int fireWish;
    public int messageLength;
}

public struct TankBattleStateData
{
    public int playerID;

    public Vector3 position;    // world position
    public Vector3 forward;     // forward position of tank

    public Vector3 cannonForward;  // forward of cannon

    public bool canFire;        // reloaded?
    public bool enemyInSight;   // is enemy within FOV of tank?
    public Vector3 lastKnownPosition;   // last known enemy position (must have LOS or fire)
    public Vector3 lastKnownDirection;  // must be in proximity and moving
}