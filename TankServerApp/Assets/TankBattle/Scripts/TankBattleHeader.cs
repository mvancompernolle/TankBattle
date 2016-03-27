using UnityEngine;
using System.Collections;

public enum TankBattleMessage
{
    NONE,
    FWRD,
    BACK,
    KILL,
    QUIT
}

public struct TankBattleHeader
{
    public int playerID;
    public TankBattleMessage msg;
    public int messageLength;
}
