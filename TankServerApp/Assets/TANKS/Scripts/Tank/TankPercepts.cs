using UnityEngine;
using System.Collections;

public class TankPercepts : MonoBehaviour
{
    public Vector3 lastKnownPosition;
    public Vector3 lastKnownDirection;

    void OnEnable()
    {
        Reset();
    }

    void Reset()
    {
        lastKnownDirection = Vector3.zero;
        lastKnownDirection = Vector3.zero;
    }

    void OnDrawGizmos()
    {
        Debug.DrawLine(transform.position, transform.position + (lastKnownDirection * 5f), Color.green);
    }
}
