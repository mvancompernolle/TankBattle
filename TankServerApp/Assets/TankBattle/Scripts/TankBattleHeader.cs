using UnityEngine;
using System.Collections.Generic;

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
    // OFFSETS                      Managed     Marshalled
    public int playerID;            // 0        0

    public Vector3 position;        // 4        4
    public Vector3 forward;         // 16       16

    public Vector3 cannonForward;   // 28       28

    public int canFire;            // 40       40

    public int perceptCount;        // 41       44  
    // http://stackoverflow.com/questions/28514373/what-is-the-size-of-a-boolean-in-c-does-it-really-take-4-bytes
    //                              // 45       48      
}