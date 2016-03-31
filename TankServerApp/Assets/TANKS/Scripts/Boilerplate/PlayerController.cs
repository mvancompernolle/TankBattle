using UnityEngine;
using System.Collections;

public class PlayerController : MonoBehaviour
{
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
