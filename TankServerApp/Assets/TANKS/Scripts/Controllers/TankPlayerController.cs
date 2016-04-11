using UnityEngine;
using UnityGame.Tanks;

public class TankPlayerController : PlayerController
{
    public IMoveable TankGun;
    public IFireable PawnFire;
    public IDamageable TankHealth;

    public TankManager manager;     // HACK: adapting Unity Tank tutorial.

    public void Fire()
    {
        PawnFire.Fire();
    }

    public void Kill()
    {
        TankHealth.TakeDamage(Mathf.Infinity);
    }

    public void RotateRight(float value)
    {
        TankGun.TurnRight(value);
    }
}
