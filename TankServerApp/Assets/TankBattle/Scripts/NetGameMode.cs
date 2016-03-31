using UnityEngine;
using System.Collections.Generic;
using System;

public class NetGameMode : MonoBehaviour
{
    private SocketListener connectionSocket;
    public int gamePort = 11000;

    // HACK: let's not couple these together
    [SerializeField]
    private UnityGame.Tanks.GameManager gameManager;

    private Dictionary<int, PlayerController> playerControllers = new Dictionary<int, PlayerController>();

    private int networkIDs = 2;

    void CheckNetworkEvents()
    {
        connectionSocket.paused = true;

        // poll and process network events
        foreach (var evnt in connectionSocket.events)
        {
            OnNetworkEvent(evnt.data, evnt.eventArgs);
        }

        connectionSocket.paused = false;

        // clear the queue
        connectionSocket.flushEvents();
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
                var msg = DataUtils.FromBytes<TankBattleHeader>(data);
                switch (msg.move)
                {
                    case Movement.FWRD:
                        Debug.Log("FWRD");
                        break;
                    case Movement.BACK:
                        Debug.Log("BACK");
                        break;
                    default:
                        Debug.Log("Unknown movement.");
                        break;
                }


                break;
            case SocketEventArgs.SocketEventType.SEND:

                break;
        }
    }

    void Start()
    {
        // listen for network players
        connectionSocket = new SocketListener();
        connectionSocket.StartListening(gamePort);
    }

    void Update()
    {
        CheckNetworkEvents();
    }

    void OnApplicationQuit()
    {
        // close socket for communication
        connectionSocket.StopListening();
    }

    // Instantiates a new player and returns its ID
    public int AddPlayer()
    {
        playerControllers[networkIDs] = gameManager.SpawnSingleTank();

        return networkIDs++; 
    }

    // Removes a player from the game by their ID
    public void RemovePlayer(int playerID)
    {

    }
}
