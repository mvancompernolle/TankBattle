#include <iostream>
#include <random>
#include <time.h>

#include "TankBattleNet.h"

#include "sfwdraw.h"
#undef NONE

int     myPlayerID = -1;

bool    isConnected = false;   
bool    isWaiting = false;
bool    isProvisioned = false;

bool    inputPressed()
{
    for (unsigned int i = 0; i < 150; ++i)
    {
        if (sfw::getKey(i))
            return true;
    }

    return false;
}

#define TANK_FWRD 'W'
#define TANK_BACK 'S'
#define TANK_LEFT 'A'
#define TANK_RIGT 'D'

#define CANN_LEFT 'Q'
#define CANN_RIGT 'E'

#define TANK_FIRE 'F'

#define GAME_QUIT 'L'

#define CLIENT_MAJOR_VERSION 0
#define CLIENT_MINOR_VERSION 1

int main(int argc, char** argv)
{
    char * serverIPAddress = "";

    // handle console arguments
    if (argc > 2)
    {
        std::cout << "Unsupported number of arguments." << std::endl;
        std::cout << "Specify the IP address of the target server. Ex: tankbattle.exe 127.0.0.1" << std::endl;

        return EXIT_FAILURE;
    }
    else if (argc == 2)
    {
        serverIPAddress = argv[1];
    }

    // Boot-up sequence
    std::cout << "TankBattleClient v" << CLIENT_MAJOR_VERSION << "." << CLIENT_MINOR_VERSION << "\n";
    
    // acquire server IP if not provided in args
    if (serverIPAddress[0] == '\0')
    {
        std::cout << "Enter the server address. \n";

        char userInput[17];
        std::cin.getline(serverIPAddress, 16);
    }

    srand(time(NULL));

    // initialize networking
    tankNet::init(serverIPAddress, 11000);


    sfw::initContext(400, 400, "TankController");
    
    while (!tankNet::isProvisioned())
    {
        tankNet::update(0.0);
    }

    auto serverData = tankNet::recieve();

    myPlayerID = serverData.playerID;


    while (sfw::stepContext() && tankNet::isConnected() && tankNet::isProvisioned())
    {
        // check TCP streams via dyad
        tankNet::update(0.0);

        auto state = tankNet::recieve();

        // prepare message
        const int msgSize = sizeof(tankBattleHeader);
        unsigned char msg[msgSize];

        tankBattleHeader ex;
        ex.msg = tankBattleMessage::NONE;
        ex.tankMove = TankMovementOptions::HALT;
        ex.cannonMove = CannonMovementOptions::HALT;
        ex.messageLength = msgSize;    // TODO: support for dynamic message length
        ex.playerID = myPlayerID;

        if (ex.playerID == -1)
            isWaiting = true;

        // poll for input
        if (inputPressed())
        {
            // tank actions
            if (sfw::getKey(TANK_FWRD))
            {
                ex.tankMove = TankMovementOptions::FWRD;
            }
            else if (sfw::getKey(TANK_BACK))
            {
                ex.tankMove = TankMovementOptions::BACK;
            }
            else if (sfw::getKey(TANK_LEFT))
            {
                ex.tankMove = TankMovementOptions::LEFT;
            }
            else if (sfw::getKey(TANK_RIGT))
            {
                ex.tankMove = TankMovementOptions::RIGHT;
            }
            else
            {
                ex.tankMove = TankMovementOptions::HALT;
            }

            if (sfw::getKey(CANN_LEFT))
            {
                ex.cannonMove = CannonMovementOptions::LEFT;
            }
            else if (sfw::getKey(CANN_RIGT))
            {
                ex.cannonMove = CannonMovementOptions::RIGHT;
            }
            else
            {
                ex.cannonMove = CannonMovementOptions::HALT;
            }

            if (sfw::getKey(TANK_FIRE))
            {
                ex.fireWish = true;
            }

            // game actions
            if (sfw::getKey(GAME_QUIT))
            {
                ex.msg = tankBattleMessage::QUIT;
                break;
            }
        }

        // begin transmission
        tankNet::send(ex);
    }

    tankNet::term();

    sfw::termContext();

    return EXIT_SUCCESS;
}