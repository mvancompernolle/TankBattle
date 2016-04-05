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

        int readCount = connectionSocket.events.Count;
        for (int i = 0; i < readCount; ++i)
        {
            try
            {
                var evnt = connectionSocket.events[i];

                if(evnt != null)
                    OnNetworkEvent(evnt.data, evnt.eventArgs);
                else
                {
                    Debug.LogError("The network event was null.");
                }
            }
            catch (Exception ex)
            {
                if(ex is NullReferenceException)
                {
                    var nullEx = ex as NullReferenceException;
                    Debug.LogError(nullEx.Message);
                }
                else if (ex is IndexOutOfRangeException)
                {
                    var indEx = ex as IndexOutOfRangeException;
                    Debug.LogError(indEx.Message);
                }
                
                Debug.LogError(ex.Message);
            }
        }

        // remove events processed
        connectionSocket.events.RemoveRange(0, readCount);

        connectionSocket.paused = false;
    }

    private void OnNetworkEvent(byte[] data, SocketEventArgs e)
    {
        switch (e.socketEvent)
        {
            case SocketEventArgs.SocketEventType.ACCEPT:
                OnNetPlayerConnected(e.endpoint);
                break;
            case SocketEventArgs.SocketEventType.READ:
                OnNetPlayerData(e.endpoint, DataUtils.FromBytes<TankBattleHeader>(data));
                break;
        }
    }

    private void OnNetPlayerConnected(NetworkPlayer netPlayer)
    {
        var PID = AddPlayer();
        var welcomeMsg = new TankBattleHeader();
        welcomeMsg.playerID = PID;
        welcomeMsg.msg = TankBattleMessage.JOIN;
        welcomeMsg.messageLength = DataUtils.SizeOf<TankBattleHeader>();

        connectionSocket.Send(netPlayer, DataUtils.GetBytes(welcomeMsg));
    }
    private void OnNetPlayerData(NetworkPlayer netPlayer, TankBattleHeader header)
    {
        if(header.playerID == -1)
        {
            Debug.LogWarning("Invalid player ID provided!");
            return;
        }

        try
        {
            // process tank movement
            switch (header.tankMove)
            {
                case TankMovementOptions.FWRD:
                    playerControllers[header.playerID].MoveForward(1.0f);
                    playerControllers[header.playerID].MoveRight(0.0f);
                    break;
                case TankMovementOptions.BACK:
                    playerControllers[header.playerID].MoveForward(-1.0f);
                    playerControllers[header.playerID].MoveRight(0.0f);
                    break;
                case TankMovementOptions.LEFT:
                    playerControllers[header.playerID].MoveForward(0.0f);
                    playerControllers[header.playerID].MoveRight(-1.0f);
                    break;
                case TankMovementOptions.RIGHT:
                    playerControllers[header.playerID].MoveForward(0.0f);
                    playerControllers[header.playerID].MoveRight(1.0f);
                    break;
                case TankMovementOptions.HALT:
                    playerControllers[header.playerID].MoveForward(0.0f);
                    playerControllers[header.playerID].MoveRight(0.0f);
                    break;
                default:
                    Debug.LogError("Unknown movement.");
                    break;
            }

            // process tank actions
            if (header.fireWish == 1)
            {
                playerControllers[header.playerID].Fire();
            }
        }
        catch (KeyNotFoundException ex)
        {
            TankBattleHeader reply = new TankBattleHeader();
            reply.playerID = AddPlayer();

            // send the reply
            connectionSocket.Send(netPlayer, reply);
        }
    }
    private void OnNetPlayerDisconnected(NetworkPlayer netPlayer)
    {
        RemovePlayer(netPlayer);
    }

    void Start()
    {
        Debug.Log("Initializing NetGameMode...");

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
        CleanUpConnections();
    }
    void OnDestroy()
    {
        CleanUpConnections();
    }

    private void CleanUpConnections()
    {
        connectionSocket.StopListening();
    }

    // Instantiates a new player and returns its ID
    public int AddPlayer()
    {
        playerControllers[networkIDs] = gameManager.SpawnSingleTank();
        return networkIDs++; 
    }

    // Removes a player from the game by their ID
    public void RemovePlayer(NetworkPlayer netPlayer)
    {

    }
}
