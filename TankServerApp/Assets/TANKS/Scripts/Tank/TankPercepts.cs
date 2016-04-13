using UnityEngine;
using UnityGame.Tanks;
using System.Collections.Generic;

public class TankPercepts : MonoBehaviour
{
    public Dictionary<int, TankTacticoolInfo> reconInfo = new Dictionary<int, TankTacticoolInfo>();

    public float VisionRadius;

    [SerializeField]
    private Transform eyePoint;
    [SerializeField]
    private float eyeFOV = 90f;

    void VisionCheck(float radarRadius)
    {
        foreach (var enemyData in new List<TankTacticoolInfo> (reconInfo.Values))
        {
            var revisedData = enemyData;
            revisedData.inSight = 0;

            reconInfo[revisedData.playerID] = revisedData;
        }

        var radar = Physics.OverlapSphere(transform.position, VisionRadius, ~(LayerMask.NameToLayer("Players")));
        foreach (var ping in radar)
        {
            if (ping.gameObject != gameObject)
            {
                var direction = (ping.transform.position - transform.position).normalized;

                RaycastHit hit;

                if (Vector3.Angle(eyePoint.forward, direction) < eyeFOV &&
                    Physics.Raycast(transform.position, direction, out hit) &&
                    hit.collider == ping)
                {
                    var tankComponent = hit.collider.GetComponent<TankMovement>();
                    var targetRecord = new TankTacticoolInfo();

                    if (reconInfo.ContainsKey(tankComponent.m_PlayerNumber))
                    {
                        targetRecord = reconInfo[tankComponent.m_PlayerNumber];
                    }
                    else
                    {
                        targetRecord = new TankTacticoolInfo();
                        targetRecord.playerID = tankComponent.m_PlayerNumber;
                    }
                    targetRecord.lastKnownDirection = hit.transform.position;
                    reconInfo[tankComponent.m_PlayerNumber] = targetRecord;
                }
            }
        }
    }

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
        VisionCheck(VisionRadius);
    }
    void OnDrawGizmos()
    {
        foreach (var enemy in reconInfo)
        {
            Debug.DrawLine(transform.position, transform.position + (enemy.Value.lastKnownDirection * 5f), Color.green);
        }
    }
    void OnGUI()
    {
        foreach(var info in reconInfo.Values)
        {
            Debug.Log(info.playerID);
        }
    }
}
