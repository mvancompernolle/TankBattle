using UnityEngine;
using System.Runtime.InteropServices;

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

public struct TankBattleCommand
{
    public TankBattleMessage msg;               // declare message type
    public TankMovementOptions tankMove;        // assign action for the tank treads
    public CannonMovementOptions cannonMove;    // TODO: implement rotation on tank gun

    [MarshalAs(UnmanagedType.I1)]
    public bool fireWish;
    public int messageLength;
}

public struct TankTacticalInfo
{
    public int playerID;

    [MarshalAs(UnmanagedType.I1)]
    public bool inSight;
    public Vector3 lastKnownPosition;
    public Vector3 lastKnownDirection;

    //public Vector3 lastKnownTankForward;
    //public Vector3 lastKnownCannonForward;
}

public struct TankBattleStateData
{
    public int playerID;
    public float currentHealth;

    public Vector3 position;
    public Vector3 forward;

    public Vector3 cannonForward;

    [MarshalAs(UnmanagedType.I1)]
    public bool canFire;

    public int perceptCount; 
}