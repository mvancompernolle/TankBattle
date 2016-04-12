using UnityEngine;
using UnityGame.Tanks;
using System.Collections.Generic;

public class TankPercepts : MonoBehaviour
{
    public Dictionary<int, TankTacticoolInfo> reconInfo = new Dictionary<int, TankTacticoolInfo>();
    public int reconInfoCount;

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
        reconInfo.Clear();
    }
    void FixedUpdate()
    {
        reconInfoCount = reconInfo.Count;
        VisionCheck(VisionRadius);
    }
    void OnDrawGizmos()
    {
        foreach (var enemy in reconInfo)
        {
            Debug.DrawLine(transform.position, transform.position + (enemy.Value.lastKnownDirection * 5f), Color.green);
        }
    }

    void VisionCheck(float radarRadius)
    {
        foreach(var enemy in reconInfo)
        {
            enemy.Value.inSight = false;
        }

        var radar = Physics.OverlapSphere(transform.position, VisionRadius, ~(LayerMask.NameToLayer("Players")));
        foreach (var ping in radar)
        {
            if (ping.gameObject != gameObject)
            {
                var direction = (ping.transform.position - transform.position).normalized;

                if (Vector3.Angle(eyePoint.forward, direction) < eyeFOV)
                {
                    RaycastHit hit;
                    if (Physics.Raycast(transform.position, direction, out hit))
                    {
                        if (hit.collider == ping)
                        {
                            var tankComponent = hit.collider.GetComponent<TankMovement>();

                            if (reconInfo.ContainsKey(tankComponent.m_PlayerNumber))
                            {
                                // update existing info
                                var reconRecord = reconInfo[tankComponent.m_PlayerNumber].lastKnownDirection = hit.transform.position;
                            }
                            else
                            {
                                var newReconRecord = new TankTacticoolInfo();
                                newReconRecord.lastKnownDirection = hit.transform.position;
                                reconInfo[tankComponent.m_PlayerNumber] = newReconRecord;
                            }
                        }
                    }
                }
            }
        }
    }
}
