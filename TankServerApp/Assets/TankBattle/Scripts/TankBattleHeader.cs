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

[System.Serializable]
public class TankTacticoolInfo
{
    public int playerID;

    public bool inSight;
    public Vector3 lastKnownPosition;
    public Vector3 lastKnownDirection;
}

public struct TankBattleStateData
{
    public int playerID;

    public Vector3 position;    // world position
    public Vector3 forward;     // forward position of tank

    public Vector3 cannonForward;  // forward of cannon

    public bool canFire;        // reloaded?

    public int perceptCount;
}