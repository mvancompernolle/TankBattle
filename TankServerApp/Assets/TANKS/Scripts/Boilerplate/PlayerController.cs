using UnityEngine;

public class PlayerController : MonoBehaviour
{
    public int pid;
    public bool isActive;

    public GameObject Pawn;

    public IMoveable MoveTarget;

    public void MoveForward(float value)
    {
        MoveTarget.MoveForward(value);
    }
    public void MoveRight(float value)
    {
        MoveTarget.TurnRight(value);
    }
}