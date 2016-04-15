#include <iostream>
#include <sstream>

#include "TankBattleNet.h"
#include "sfwdraw.h"
#undef NONE

using std::stringstream;
using namespace tankNet;

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

const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH = 400;

int main(int argc, char** argv)
{
    char * serverIPAddress = "";

    // handle console arguments
    if (argc > 2)
    {
        std::cout << "Unsupported number of arguments." << std::endl;
        std::cout << "Specify the IP address of the target server. Ex: " << argv[0] << " 127.0.0.1" << std::endl;

        return EXIT_FAILURE;
    }
    else if (argc == 2)
    {
        serverIPAddress = argv[1];
    }

    // initialize networking
    if (serverIPAddress[0] == '\0')
    {
        tankNet::init(11000);
    }
    else
    {
        tankNet::init(11000, serverIPAddress);
    }

    while (!tankNet::isProvisioned())
    {
		// block further execution until the server responds
		// or until the client gives up on connecting
        tankNet::update();
    }

	auto serverData = tankNet::recieve();
    myPlayerID = serverData->playerID;

	// initialize SFW and assets
    sfw::initContext(WINDOW_WIDTH, WINDOW_HEIGHT, "TankController");
	unsigned font = sfw::loadTextureMap("./res/fontmap.png", 16, 16);

    while (sfw::stepContext() && tankNet::isConnected() && tankNet::isProvisioned())
    {
        // check TCP streams via dyad
        tankNet::update();
		if (tankNet::isConnected() == false)
		{
			break;
		}

        TankBattleStateData * state = tankNet::recieve();
        
		// diagnostic report of current state
		stringstream debugStrings;
		debugStrings << *state;
		debugStrings << "Tacticool Report:\n  ";
		for (int i = 0; i < state->tacticoolCount; ++i)
		{
			debugStrings << state->tacticoolData[i].playerID << "\n    ";
			for (int j = 0; j < 3; ++j)
			{
				debugStrings << state->tacticoolData[i].lastKnownPosition[j] << " , ";
			}
			debugStrings << "\n    ";
			for (int j = 0; j < 3; ++j)
			{
				debugStrings << state->tacticoolData[i].lastKnownDirection[j] << " , ";
			}
			debugStrings << "inSight: " << (state->tacticoolData[i].inSight ? "true" : "false") << "\n";
		}

		sfw::drawString(font, debugStrings.str().c_str(), 0, WINDOW_HEIGHT, 15, 15);

        // prepare message
        TankBattleHeader ex;
        ex.msg = TankBattleMessage::NONE;
        ex.tankMove = TankMovementOptions::HALT;
        ex.cannonMove = CannonMovementOptions::HALT;

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
                ex.msg = TankBattleMessage::QUIT;
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