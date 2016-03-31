using UnityEngine;
using System.Collections;

public enum TankBattleMessage
{
    NONE,
    LOGIN,
    QUIT
}

public enum Movement
{
    HALT,
    FWRD,
    BACK,
    LEFT,
    RIGHT
}

public struct TankBattleHeader
{
    public int playerID;
    public TankBattleMessage msg;
    public Movement move;
    public int messageLength;
}
