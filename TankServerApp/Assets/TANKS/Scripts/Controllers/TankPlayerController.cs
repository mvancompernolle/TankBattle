using UnityEngine;

public class TankPlayerController : PlayerController
{
    public IMoveable TankGun;
    public IFireable PawnFire;
    public IDamageable TankHealth;

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
