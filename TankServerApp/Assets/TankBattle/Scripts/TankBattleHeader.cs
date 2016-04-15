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

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct TankBattleCommand
{
    public TankBattleMessage msg;               // declare message type
    public TankMovementOptions tankMove;        // assign action for the tank treads
    public CannonMovementOptions cannonMove;    // TODO: implement rotation on tank gun

    [MarshalAs(UnmanagedType.I1)]
    public bool fireWish;
    public int messageLength;
}

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct TankTacticalInfo
{
    public int playerID;                        // 0

    [MarshalAs(UnmanagedType.I1)]
    public bool inSight;                        // 1
    public Vector3 lastKnownPosition;           // 13
    public Vector3 lastKnownDirection;          // 25

    //public Vector3 lastKnownTankForward;
    //public Vector3 lastKnownCannonForward;
}

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct TankBattleStateData
{
    public int messageLength;       // 0

    public int playerID;            // 4
    public float currentHealth;     // 8

    public Vector3 position;        // 12
    public Vector3 forward;         // 24

    public Vector3 cannonForward;   // 36

    [MarshalAs(UnmanagedType.I1)]
    public bool canFire;            // 48

    public int perceptCount;        // 49
                                    // 53
}