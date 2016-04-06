using UnityEngine;
using System.Collections;

public class PlayerController : MonoBehaviour
{
    // pawn controlled by this controller
    public IMoveable Pawn;
    public IFireable PawnFire;

    public void Fire()
    {
        PawnFire.Fire();
    }

    public void MoveForward(float value)
    {
        Pawn.MoveForward(value);
    }
    public void MoveRight(float value)
    {
        Pawn.TurnRight(value);
    }
}