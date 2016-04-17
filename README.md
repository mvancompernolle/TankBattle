# TankBattle

TankBattle provides a base project and client lib for facilitating communication
between a server and a client in order to run a networked simulation. There are
three projects provided in this repository:

- TankServerApp
- TankClientLib
  - TankBattle-net
  - TankBattle-dyad-sfw

TankServerApp contains the Unity3D project which serves as the network server as
well as the simulation. Clients will send commands to the server over TCP sockets
and the server will process them every FixedUpdate. After processing, the server
will update each client, sending everyone a new snapshot of the current state of
their tank. Its codebase is compiled against _Unity3D v5.3.2f1_.

TankBattle-net contains the basic networking library for sending commands to the
server and receiving state updates from the server. Under the hood, the library
wraps dyad, a C library for handling TCP sockets.

TankBattle-dyad-sfw contains the example client for use in connecting with the
server. It accepts keyboard input for controlling the tank and displays a
debug output in the game window, showing what information is received from the
server.

## Getting Started

## Connecting the Basic Server and Client

1. Open the Unity3D project located in the TankServerApp folder.
2. Hit play, and verify that there are no problems with initialization by looking
at the console output.
3. Open up the solution for the TankClientLib.
4. Build the solution. The build order should be configured to build TankBattle-net
first, which is a dependency of TankBattle-dyad-sfw. Upon successfully building,
start debugging to fire up the startup project which, by default, connects to
127.0.0.1, or localhost.
5. You should be able to focus the client game window and use the WASD-keys to move
around on the server.
6. To spin up a second client, right-click on the TankBattle-dyad-sfw project in
Visual Studio's Solution Explorer and hover over the "Debug" option. There should be
a menu entry for starting a new instance. Both of these instances will be picked up
by the debugger.  

## License

This project is licensed under the [MIT/X11 License](LICENSE.md).
