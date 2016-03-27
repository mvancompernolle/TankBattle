using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

using System.Collections.Generic;
using System.Runtime.InteropServices;

using UnityEngine;

public class NetworkPlayer
{
    public Socket line;

    public static implicit operator Socket(NetworkPlayer p)
    {
        return p.line;
    }
}

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
        SEND
    }
    public readonly SocketEventType socketEvent;
    public readonly NetworkPlayer endpoint;

    public SocketEventArgs(SocketEventType e, NetworkPlayer remote)
    {
        socketEvent = e;
        endpoint = remote;
    }
}
public delegate void SocketEventHandler(byte[] data, SocketEventArgs e);

// TODO: need a way to get messages from this to something else
// - perhaps an event?

public static class DataUtils
{
    public static int SizeOf<T>()
    {
        return Marshal.SizeOf(typeof(T));
    }
    public static int GetSize(this object obj)
    {
        return Marshal.SizeOf(obj);
    }

    public static byte[] GetBytes(object obj)
    {
        int size = Marshal.SizeOf(obj);
        IntPtr ptr = Marshal.AllocHGlobal(size);

        byte[] data = new byte[size];
        Marshal.StructureToPtr(obj, ptr, true);
        Marshal.Copy(ptr, data, 0, size);
        Marshal.FreeHGlobal(ptr);

        return data;
    }
    public static T FromBytes<T>(byte[] data)
    {
        T copy = default(T);

        int tSize = Marshal.SizeOf(copy);
        IntPtr ptr = Marshal.AllocHGlobal(tSize);

        Marshal.Copy(data, 0, ptr, tSize);
        copy = (T)Marshal.PtrToStructure(ptr, copy.GetType());
        Marshal.FreeHGlobal(ptr);

        return copy;
    }
}

// Setup a listener on a given port
public class SocketListener
{
    Socket localListener;
    List<StateObject> remoteConnections = new List<StateObject>();
    Dictionary<StateObject, NetworkPlayer> players = new Dictionary<StateObject, NetworkPlayer>();

    public int port { get; private set; }
    private ManualResetEvent allDone = new ManualResetEvent(false);

    public event SocketEventHandler socketTransmission;

    public void Send(Socket handler, String data)
    {
        byte[] byteData = Encoding.ASCII.GetBytes(data);

        handler.BeginSend(byteData, 0, byteData.Length, 0,
            new AsyncCallback(SendCallback), handler);
    }
    public void Send(Socket handler, byte[] data)
    {
        handler.BeginSend(data, 0, data.Length, 0,
            new AsyncCallback(SendCallback), handler);
    }
    public void Send(Socket handler, object data)
    {
        Send(handler, DataUtils.GetBytes(data));
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

        NetworkPlayer netPlayer = new NetworkPlayer();
        netPlayer.line = handler;

        players[state] = netPlayer;

        if (socketTransmission != null)
        { socketTransmission.Invoke(null, new SocketEventArgs(SocketEventArgs.SocketEventType.ACCEPT, netPlayer)); }

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
            // TODO: Can we add generics to callbacks?
            // How can we simplify this to be reusable?
            if (state.buffer.Length >= DataUtils.SizeOf<TankBattleHeader>())
            {
                Console.WriteLine("Read {0} bytes from socket.", bytesRead);

                var data = DataUtils.FromBytes<TankBattleHeader>(state.buffer);
                Debug.Log(data.msg);

                Debug.Log("Message recieved.");

                if (socketTransmission != null)
                { socketTransmission.Invoke(state.buffer, new SocketEventArgs(SocketEventArgs.SocketEventType.READ, players[state])); }
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
            StateObject state = (StateObject)ar.AsyncState;

            Socket handler = (Socket)ar.AsyncState;

            int bytesSent = handler.EndSend(ar);
            Console.WriteLine("Sent {0} bytes to client.", bytesSent);

            if (socketTransmission != null)
            { socketTransmission.Invoke(null, new SocketEventArgs(SocketEventArgs.SocketEventType.SEND, players[state])); }
        }
        catch (Exception e)
        {
            Console.WriteLine(e.ToString());
        }

        Debug.Log("Message sent.");
    }

    public void StartListening(int port)
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
    public void StopListening()
    {
        // close local socket
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

        // close sockets to other machines
        Debug.LogFormat("Closing {0} sockets to remote machines.", remoteConnections.Count);
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