#include <iostream>

#include "TankBattleNet.h"
#include "sfwdraw.h"
#undef NONE

int myPlayerID = -1;

bool inputPressed()
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

    // initialize networking
    if (serverIPAddress[0] == '\0')
    {
        tankNet::init(11000);
    }
    else
    {
        tankNet::init(11000, serverIPAddress);
    }

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

        // poll for input
        if (inputPressed())
        {
            // tank actions
            ex.tankMove =   sfw::getKey(TANK_FWRD) ? TankMovementOptions::FWRD :
                            sfw::getKey(TANK_BACK) ? TankMovementOptions::BACK :
                            sfw::getKey(TANK_LEFT) ? TankMovementOptions::LEFT :
                            sfw::getKey(TANK_RIGT) ? TankMovementOptions::RIGHT :
                            TankMovementOptions::HALT;

            ex.cannonMove = sfw::getKey(CANN_LEFT) ? CannonMovementOptions::LEFT :
                            sfw::getKey(CANN_RIGT) ? CannonMovementOptions::RIGHT :
                            CannonMovementOptions::HALT;

            ex.fireWish = sfw::getKey(TANK_FIRE);

            // game actions
            if (sfw::getKey(GAME_QUIT))
            {
                ex.msg = tankBattleMessage::QUIT;
            }
        }

        // begin transmission
        tankNet::send(ex);
    }

    tankNet::term();

    sfw::termContext();

    return EXIT_SUCCESS;
}