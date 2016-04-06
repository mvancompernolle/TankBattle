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

    private Dictionary<int, TankPlayerController> playerControllers = new Dictionary<int, TankPlayerController>();
    private Dictionary<int, NetworkPlayer> networkPlayersByPID = new Dictionary<int, NetworkPlayer>();

    private int networkIDs = 2;

    // HACK: Unity API is not thread-safe, so can't subscribe to delegates that get trigger asynchronously
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
                    Debug.LogWarning("The network event was null.");
                    // not sure why this happens

                    continue;
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
    void UpdateClients()
    {
        foreach(var netPlayer in networkPlayersByPID)
        {
            var player = playerControllers[netPlayer.Key];

            var stateMsg = new TankBattleStateData();
            stateMsg.playerID = netPlayer.Key;
            stateMsg.position = player.Pawn.position;
            stateMsg.forward  = player.Pawn.forward;
            stateMsg.cannonForward = player.TankGun.forward;
            stateMsg.canFire  = player.PawnFire.CanFire();
            stateMsg.enemyInSight = false;

            connectionSocket.Send(netPlayer.Value, stateMsg);
        }
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
        var welcomeMsg = new TankBattleStateData();
        welcomeMsg.playerID = PID;

        networkPlayersByPID[PID] = netPlayer;

        connectionSocket.Send(netPlayer, DataUtils.GetBytes(welcomeMsg));

        Debug.Log("New player initialized at ID" + PID);
    }
    private void OnNetPlayerData(NetworkPlayer netPlayer, TankBattleHeader header)
    {
        if(header.playerID == -1)
        {
            Debug.LogWarning("Invalid player ID provided!");
            return;
        }

        switch(header.msg)
        {
            case TankBattleMessage.QUIT:
                RemovePlayer(netPlayer);
                break;
            default:
                break;
        }

        try
        {
            var pc = playerControllers[header.playerID];

            // process tank movement
            switch (header.tankMove)
            {
                case TankMovementOptions.FWRD:
                    pc.MoveForward(1.0f);
                    pc.MoveRight(0.0f);
                    break;
                case TankMovementOptions.BACK:
                    pc.MoveForward(-1.0f);
                    pc.MoveRight(0.0f);
                    break;
                case TankMovementOptions.LEFT:
                    pc.MoveForward(0.0f);
                    pc.MoveRight(-1.0f);
                    break;
                case TankMovementOptions.RIGHT:
                    pc.MoveForward(0.0f);
                    pc.MoveRight(1.0f);
                    break;
                case TankMovementOptions.HALT:
                    pc.MoveForward(0.0f);
                    pc.MoveRight(0.0f);
                    break;
                default:
                    Debug.LogError("Unknown movement.");
                    break;
            }

            switch(header.cannonMove)
            {
                case CannonMovementOptions.LEFT:
                    pc.RotateRight(-1.0f);
                    break;
                case CannonMovementOptions.RIGHT:
                    pc.RotateRight(1.0f);
                    break;
                case CannonMovementOptions.HALT:
                    pc.RotateRight(0.0f);
                    break;
                default:
                    Debug.LogError("Unknown cannon movement.");
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
    void FixedUpdate()
    {
        UpdateClients();
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
        netPlayer.isActive = false;

        netPlayer.remoteSocket.Shutdown(System.Net.Sockets.SocketShutdown.Both);
        netPlayer.remoteSocket.Disconnect(true);
        netPlayer.remoteSocket.Close();
    }
}
