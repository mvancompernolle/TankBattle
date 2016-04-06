using UnityEngine;
using System.Collections;

public class TankPlayerController : PlayerController
{
    public IMoveable TankGun;

    public void RotateRight(float value)
    {
        TankGun.TurnRight(value);
    }
}
