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
            var evnt = connectionSocket.events[i];
            OnNetworkEvent(evnt.data, evnt.eventArgs);
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
                break;
            case SocketEventArgs.SocketEventType.READ:
                var msg = DataUtils.FromBytes<TankBattleHeader>(data);

                //switch(msg.msg)
                //{
                //    case TankBattleMessage.NONE:
                //        break;
                //    case TankBattleMessage.JOIN:
                //        TankBattleHeader reply = new TankBattleHeader();
                //        reply.playerID = AddPlayer();

                //        // send the reply
                //        connectionSocket.Send(e.endpoint, reply);
                //        break;
                //    default:
                //        break;
                //}

                if (msg.playerID == -1)
                {
                    Debug.LogWarning("New player registered.");
                    TankBattleHeader reply = new TankBattleHeader();
                    reply.playerID = AddPlayer();

                    // send the reply
                    connectionSocket.Send(e.endpoint, reply);
                }
                else
                {
                    try
                    {
                        switch (msg.tankMove)
                        {
                            case TankMovementOptions.FWRD:
                                playerControllers[msg.playerID].MoveForward(1.0f);
                                playerControllers[msg.playerID].MoveRight(0.0f);
                                break;
                            case TankMovementOptions.BACK:
                                playerControllers[msg.playerID].MoveForward(-1.0f);
                                playerControllers[msg.playerID].MoveRight(0.0f);
                                break;
                            case TankMovementOptions.LEFT:
                                playerControllers[msg.playerID].MoveForward(0.0f);
                                playerControllers[msg.playerID].MoveRight(-1.0f);
                                break;
                            case TankMovementOptions.RIGHT:
                                playerControllers[msg.playerID].MoveForward(0.0f);
                                playerControllers[msg.playerID].MoveRight(1.0f);
                                break;
                            case TankMovementOptions.HALT:
                                playerControllers[msg.playerID].MoveForward(0.0f);
                                playerControllers[msg.playerID].MoveRight(0.0f);
                                break;
                            default:
                                Debug.LogError("Unknown movement.");
                                break;
                        }

                        if(msg.fireWish == 1)
                        {
                            playerControllers[msg.playerID].Fire();
                        }
                    }
                    catch (KeyNotFoundException ex)
                    {
                        TankBattleHeader reply = new TankBattleHeader();
                        reply.playerID = AddPlayer();

                        // send the reply
                        connectionSocket.Send(e.endpoint, reply);
                    }
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
