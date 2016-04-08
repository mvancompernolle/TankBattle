public class TankPlayerController : PlayerController
{
    public IMoveable TankGun;

    public UnityGame.Tanks.TankManager manager; // HACK: :((((

    public void RotateRight(float value)
    {
        TankGun.TurnRight(value);
    }
}
