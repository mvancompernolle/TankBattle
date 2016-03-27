using UnityEngine;
using System.Collections.Generic;
using System.Collections;

public class GameMode : MonoBehaviour
{
    enum MatchState
    {
        WAITING_FOR_PLAYERS,
        IN_PROGRESS,
        WAITING_FOR_RESTART
    }

    [SerializeField]
    private SocketListener connectionSocket;

    [SerializeField]
    private MatchState currentState;    // current state of the match
    private int playerIDQueue;          // next player ID to allocate

    // stores a mapping between playerID and controller
    private Dictionary<int, GameObject> playerControllers = new Dictionary<int, GameObject>();

    // TODO: need a script to associate the two w/ each other
    [SerializeField]
    private GameObject PlayerControllerPrefab;

    [SerializeField]
    private GameObject PawnPrefab;

    public List<Transform> playerStarts = new List<Transform>();

    // Returns a random transform the the list of starting locations
    // - if none, returns a transform referring to the origin of the world
    public Transform GetPlayerStart()
    {
        if (playerStarts.Count < 1)
        {
            return new GameObject("PlayerStart").transform;
        }
        else
        {
            return playerStarts[(Random.Range(0, playerStarts.Count - 1))];
        }
    }

    public int AddPlayer()
    {
        int newPlayerID = playerIDQueue++;

        Transform startingLocation = GetPlayerStart();
        var newPawn = Instantiate(PawnPrefab, startingLocation.position, startingLocation.rotation ); 

        return newPlayerID;
    }

    public void RemovePlayer(int playerID)
    {

    }

    void Start()
    {
        // listen to the socket listener for events
        connectionSocket.socketTransmission += OnNetworkEvent;
    }

    private void OnNetworkEvent(byte[] data, SocketEventArgs e)
    {
        switch (e.socketEvent)
        {
            case SocketEventArgs.SocketEventType.ACCEPT:
                TankBattleHeader reply = new TankBattleHeader();
                reply.playerID = AddPlayer();

                // send the reply
                connectionSocket.Send(e.endpoint, DataUtils.GetBytes(reply));
                
                break;
            case SocketEventArgs.SocketEventType.READ:

                break;
            case SocketEventArgs.SocketEventType.SEND:

                break;
        }
    }
}
