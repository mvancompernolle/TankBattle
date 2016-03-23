using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

using System.Collections.Generic;
using System.Runtime.InteropServices;

using UnityEngine;



public class StateObject
{
    public Socket workSocket = null;

    public const int BufferSize = 1024;

    public byte[] buffer = new byte[BufferSize];
}

public class SocketEventArgs : EventArgs
{
    public enum SocketEventType
    {
        ACCEPT,
        READ,
    }

    SocketEventArgs socketEvent;
}
public delegate void SocketEventHandler(byte[] data, SocketEventArgs e);

public enum TankBattleMessage
{
    NONE,
    FWRD,
    BACK,
    KILL,
    QUIT
}

public struct TankBattleHeader
{
    public int playerID;
    public TankBattleMessage msg;
    public int messageLength;
}

// TODO: need a way to get messages from this to something else
// - perhaps an event?

public static class DataUtils
{
    public static int SizeOf<T>()
    {
        return System.Runtime.InteropServices.Marshal.SizeOf(typeof(T));
    }

    public static int GetSize(this object obj)
    {
        return System.Runtime.InteropServices.Marshal.SizeOf(obj);
    }
}

// Setup a listener on a given port
public class SocketListener : MonoBehaviour
{
    Socket localListener;
    List<StateObject> remoteConnections = new List<StateObject>();

    public int port;
    public static ManualResetEvent allDone = new ManualResetEvent(false);

    public event SocketEventHandler socketTransmission;

    T RebuildData<T>(byte[] data) where T : struct
    {
        T copy = new T();

        int tSize = Marshal.SizeOf(copy);
        IntPtr ptr = Marshal.AllocHGlobal(tSize);

        Marshal.Copy(data, 0, ptr, tSize);
        copy = (T)Marshal.PtrToStructure(ptr, copy.GetType());
        Marshal.FreeHGlobal(ptr);

        return copy;
    }

    private void Send(Socket handler, String data)
    {
        byte[] byteData = Encoding.ASCII.GetBytes(data);

        handler.BeginSend(byteData, 0, byteData.Length, 0,
            new AsyncCallback(SendCallback), handler);
    }

    private void AcceptCallback(IAsyncResult ar)
    {
        allDone.Set();

        Socket listener = (Socket)ar.AsyncState;
        Socket handler = listener.EndAccept(ar);

        Debug.Log("Connection from " + ((IPEndPoint)handler.RemoteEndPoint).Address.ToString());

        StateObject state = new StateObject();
        state.workSocket = handler;

        remoteConnections.Add(state);

        handler.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0,
            new AsyncCallback(ReadCallback), state);
    }
    private void ReadCallback(IAsyncResult ar)
    {
        StateObject state = (StateObject)ar.AsyncState;
        Socket handler = state.workSocket;

        int bytesRead = handler.EndReceive(ar);

        Debug.Log("Read " + bytesRead + " bytes ...");

        if (bytesRead > 0)
        {
            if (state.buffer.Length >= DataUtils.SizeOf<TankBattleHeader>())
            {
                Console.WriteLine("Read {0} bytes from socket.", bytesRead);

                var data = RebuildData<TankBattleHeader>(state.buffer);
                Debug.Log(data.msg);

                Debug.Log("Message recieved.");

                Send(handler, "OK\n");
            }
            else
            {
                Debug.Log("Need more information.");
            }

            // wipe out state
            Array.Clear(state.buffer, 0, state.buffer.Length);

            handler.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0,
    new AsyncCallback(ReadCallback), state);
        }
    }
    private void SendCallback(IAsyncResult ar)
    {
        try
        {
            Socket handler = (Socket)ar.AsyncState;

            int bytesSent = handler.EndSend(ar);
            Console.WriteLine("Sent {0} bytes to client.", bytesSent);

            //handler.Shutdown(SocketShutdown.Both);
            //handler.Close();
        }
        catch (Exception e)
        {
            Console.WriteLine(e.ToString());
        }

        Debug.Log("Message sent.");
    }

    void InitializeSocket(int port)
    {
        IPHostEntry ipHostInfo = Dns.Resolve(Dns.GetHostName());
        IPAddress ipAddress = ipHostInfo.AddressList[0];

        
        IPEndPoint localEndPoint = new IPEndPoint(ipAddress, port);

        Socket listener = new Socket(AddressFamily.InterNetwork,
                SocketType.Stream, ProtocolType.Tcp);

        localListener = listener;
        try
        {
            Debug.LogFormat("Binding to <{0}>.", ipHostInfo.AddressList[0]);
            listener.Bind(localEndPoint);
            listener.Listen(100);

            allDone.Reset();

            listener.BeginAccept(new AsyncCallback(AcceptCallback), listener);
            Debug.Log("Ready to accept connections.");
        }
        catch (Exception e)
        {
            Debug.LogError(e.ToString());
        }

        Debug.Log("Socket Initialized.");
        Debug.LogFormat("Listening on port <{0}>.", port);
    }

    void Start()
    {
        InitializeSocket(port);
    }

    void OnApplicationQuit()
    {
        Debug.LogWarning("HI");
        if (localListener != null)
        {
            try
            {
                localListener.Shutdown(SocketShutdown.Both);
                localListener.Close();
            }
            catch (Exception e)
            {
                Debug.LogError(e.Message);
            }
        }

        Debug.LogFormat("Closing {0} sockets to external contacts.", remoteConnections.Count);
        for(int i = 0; i < remoteConnections.Count; ++i)
        {
            try
            {
                var connSock = remoteConnections[i].workSocket;
                connSock.Shutdown(SocketShutdown.Both);
                connSock.Close();
            }
            catch (Exception e)
            {
                Debug.LogError(e.Message);
            }
        }
    }
}