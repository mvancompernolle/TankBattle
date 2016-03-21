using System;
using System.Collections;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;

public class StateObject
{
    public Socket workSocket = null;

    public const int BufferSize = 1024;

    public byte[] buffer = new byte[BufferSize];

    public StringBuilder sb = new StringBuilder();
}
//buttts
public class SocketListener : MonoBehaviour
{
    public int port;
    public static ManualResetEvent allDone = new ManualResetEvent(false);

    private static void Send(Socket handler, String data)
    {
        byte[] byteData = Encoding.ASCII.GetBytes(data);

        handler.BeginSend(byteData, 0, byteData.Length, 0,
            new AsyncCallback(SendCallback), handler);
    }

    private static void AcceptCallback(IAsyncResult ar)
    {
        Debug.Log("Connection Established.");

        allDone.Set();

        Socket listener = (Socket)ar.AsyncState;
        Socket handler = listener.EndAccept(ar);

        Debug.Log(((IPEndPoint)handler.RemoteEndPoint).Address.ToString());

        StateObject state = new StateObject();
        state.workSocket = handler;
        handler.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0,
            new AsyncCallback(ReadCallback), state);
    }
    private static void ReadCallback(IAsyncResult ar)
    {
        String content = String.Empty;

        StateObject state = (StateObject)ar.AsyncState;
        Socket handler = state.workSocket;

        int bytesRead = handler.EndReceive(ar);

        if (bytesRead > 0)
        {
            state.sb.Append(Encoding.ASCII.GetString(state.buffer, 0, bytesRead));

            content = state.sb.ToString();
            if (content.IndexOf("<EOF>") > -1)
            {
                Console.WriteLine("Read {0} bytes from socket. \n Data : {1}",
                    content.Length, content);

                Send(handler, content);
            }
            else
            {
                handler.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0,
                    new AsyncCallback(ReadCallback), state);
            }
        }

        Debug.Log("Message recieved.");
    }
    private static void SendCallback(IAsyncResult ar)
    {
        try
        {
            Socket handler = (Socket)ar.AsyncState;

            int bytesSent = handler.EndSend(ar);
            Console.WriteLine("Sent {0} bytes to client.", bytesSent);

            handler.Shutdown(SocketShutdown.Both);
            handler.Close();
        }
        catch (Exception e)
        {
            Console.WriteLine(e.ToString());
        }

        Debug.Log("Message sent.");
    }

    IEnumerator startListening(Socket listener)
    {
        allDone.Reset();

        Debug.Log("Waiting for a connection...");
        listener.BeginAccept(new AsyncCallback(AcceptCallback), listener);

        //allDone.WaitOne();

        yield return null;
    }

    void InitializeSocket(int port)
    {
        IPHostEntry ipHostInfo = Dns.Resolve(Dns.GetHostName());
        IPAddress ipAddress = ipHostInfo.AddressList[0];
        IPEndPoint localEndPoint = new IPEndPoint(ipAddress, port);

        Socket listener = new Socket(AddressFamily.InterNetwork,
                SocketType.Stream, ProtocolType.Tcp);

        try
        {
            listener.Bind(localEndPoint);
            listener.Listen(100);

            StartCoroutine(startListening(listener));
            //while (true)
            //{
            //    allDone.Reset();

            //    Debug.Log("Waiting for a connection...");
            //    listener.BeginAccept(new AsyncCallback(AcceptCallback), listener);

            //    allDone.WaitOne();
            //}
        }
        catch (Exception e)
        {
            Debug.LogError(e.ToString());
        }

        Debug.Log("Socket Initialized.");
    }

    void Start()
    {
        InitializeSocket(11000);
    }
}