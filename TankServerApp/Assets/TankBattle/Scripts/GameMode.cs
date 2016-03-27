using UnityEngine;
using System.Collections.Generic;
using System;

public class GameMode : MonoBehaviour
{
    enum MatchState
    {
        WAITING_FOR_PLAYERS,
        IN_PROGRESS,
        WAITING_FOR_RESTART
    }

    private SocketListener connectionSocket;
    public int gamePort = 11000;

    [SerializeField] [ReadOnly]
    private MatchState currentState;    // current state of the match
    private int playerIDQueue;          // next player ID to allocate

    // stores a mapping between playerID and controller
    private Dictionary<int, GameObject> playerControllers = new Dictionary<int, GameObject>();

    // TODO: need a way to associate the two w/ each other
    [SerializeField]
    private GameObject PlayerControllerPrefab;

    [SerializeField]
    private GameObject PawnPrefab;

    public List<Transform> playerStarts = new List<Transform>();

    void Start()
    {
        // listen for network players
        connectionSocket = new SocketListener();
        connectionSocket.StartListening(gamePort);
        //connectionSocket.socketTransmission += OnNetworkEvent;    // must use Unity API from main thread
    }

    void Update()
    {
        // poll and process network events
        foreach (var evnt in connectionSocket.events)
        {
            OnNetworkEvent(evnt.data, evnt.eventArgs);
        }

        // clear the queue
        connectionSocket.flushEvents();
    }

    void OnApplicationQuit()
    {
        // close socket for communication
        connectionSocket.StopListening();
    }

    // Returns a random transform the the list of starting locations
    // - if none, returns a transform referring to the origin of the world
    public Transform GetPlayerStart()
    {
        Transform spawnTransform;

        try
        {
            spawnTransform = playerStarts[(UnityEngine.Random.Range(0, playerStarts.Count - 1))];
        }
        catch (Exception e)
        {
            Debug.LogWarning(e.Message);

            spawnTransform = new GameObject("PlayerStart").transform;
        }

        return spawnTransform;
    }

    // Instantiates a new player and returns its ID
    public int AddPlayer()
    {
        int newPlayerID = playerIDQueue++;

        Transform startingLocation = GetPlayerStart();
        var newPawn = Instantiate(PawnPrefab, startingLocation.position, startingLocation.rotation ); 

        return newPlayerID;
    }
    // Removes a player from the game by their ID
    public void RemovePlayer(int playerID)
    {

    }

    private void OnNetworkEvent(byte[] data, SocketEventArgs e)
    {
        switch (e.socketEvent)
        {
            case SocketEventArgs.SocketEventType.ACCEPT:
                TankBattleHeader reply = new TankBattleHeader();
                reply.playerID = AddPlayer();

                // send the reply
                connectionSocket.Send(e.endpoint, reply);
                
                break;
            case SocketEventArgs.SocketEventType.READ:

                break;
            case SocketEventArgs.SocketEventType.SEND:

                break;
        }
    }
}
