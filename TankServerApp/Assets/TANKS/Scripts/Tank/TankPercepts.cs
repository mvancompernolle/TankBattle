using UnityEngine;
using System.Collections;

public class TankPercepts : MonoBehaviour
{
    //[HideInInspector]
    public Vector3 lastKnownPosition;
    //[HideInInspector]
    public Vector3 lastKnownDirection;

    public float VisionRadius;

    [SerializeField]
    private Transform eyePoint;
    [SerializeField]
    private float eyeFOV = 90f;

    void OnEnable()
    {
        Reset();
    }

    void Reset()
    {
        lastKnownDirection = Vector3.zero;
        lastKnownDirection = Vector3.zero;
    }

    void FixedUpdate()
    {
        var radar = Physics.OverlapSphere(transform.position, VisionRadius, ~(LayerMask.NameToLayer("Players")));
        foreach(var ping in radar)
        {
            if(ping.gameObject != gameObject)
            {
                var direction = (ping.transform.position - transform.position).normalized;

                if(Vector3.Angle(eyePoint.forward, direction) < eyeFOV)
                {
                    RaycastHit hit;
                    if(Physics.Raycast(transform.position, direction, out hit))
                    {
                        if (hit.collider == ping)
                        {
                            lastKnownPosition = hit.transform.position;
                        }
                    }
                }
            }
        }
    }

    void OnDrawGizmos()
    {
        Debug.DrawLine(transform.position, transform.position + (lastKnownDirection * 5f), Color.green);
    }
}
