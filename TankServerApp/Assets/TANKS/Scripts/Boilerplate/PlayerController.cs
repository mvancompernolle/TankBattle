using UnityEngine;

public class PlayerController : MonoBehaviour
{
    public int pid;
    public bool isActive;
    public IMoveable Pawn;

    public void MoveForward(float value)
    {
        Pawn.MoveForward(value);
    }
    public void MoveRight(float value)
    {
        Pawn.TurnRight(value);
    }
}