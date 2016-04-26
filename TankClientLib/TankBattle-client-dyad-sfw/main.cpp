#include <iostream>
#include <sstream>

#include "TankBattleNet.h"
#include "sfwdraw.h"
#include "Agent.h"
#undef NONE     // sfw defines NONE as one of its colors; we won't be needing that

using std::stringstream;

// declare controls for example client
const char TANK_FWRD = 'W';
const char TANK_BACK = 'S';
const char TANK_LEFT = 'A';
const char TANK_RIGT = 'D';

const char TANK_FIRE = 'F';

const char CANN_LEFT = 'Q';
const char CANN_RIGT = 'E';

const char GAME_QUIT = 'L';

const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH = 400;

// Polls all printable characters for a key press
// Returns true if a key press was detected, otherwise returns false
bool inputPressed()
{
    // 32 is the ASCII code for the first printable character, while 255 is the last
    for (unsigned int i = 32; i < 255; ++i)
    {
        if (sfw::getKey(i))
            return true;
    }

    return false;
}

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
        tankNet::init();
    }
    else
    {
        tankNet::init(serverIPAddress);
    }

    while (!tankNet::isProvisioned())
    {
		// block further execution until the server responds
		// or until the client gives up on connecting
        tankNet::update();
    }

    // if a connection was successful...
    if (tankNet::isConnected() && tankNet::isProvisioned())
    {
		bool playerControlled = false;
		Agent agent;
        auto serverData = tankNet::recieve();

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

            tankNet::TankBattleStateData * state = tankNet::recieve();

            // diagnostic report of current state
            stringstream debugStrings;
            debugStrings << *state;
            debugStrings << "Tacticool Report:\n";
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

                debugStrings << "\ninSight: " << (state->tacticoolData[i].inSight ? "true" : "false") << "\n";
            }

            sfw::drawString(font, debugStrings.str().c_str(), 0, WINDOW_HEIGHT, 15, 15);

            // prepare message
            tankNet::TankBattleCommand ex;

			// init tank command
			ex.msg = tankNet::TankBattleMessage::NONE;
			ex.tankMove = tankNet::TankMovementOptions::HALT;
			ex.cannonMove = tankNet::CannonMovementOptions::HALT;

            // poll for input
            if (inputPressed())
            {
				if ( sfw::getKey( 'P' ) ) {
					playerControlled = !playerControlled;
				}

				if ( playerControlled ) {

					// tank actions
					ex.tankMove = sfw::getKey( TANK_FWRD ) ? tankNet::TankMovementOptions::FWRD :
						sfw::getKey( TANK_BACK ) ? tankNet::TankMovementOptions::BACK :
						sfw::getKey( TANK_LEFT ) ? tankNet::TankMovementOptions::LEFT :
						sfw::getKey( TANK_RIGT ) ? tankNet::TankMovementOptions::RIGHT :
						tankNet::TankMovementOptions::HALT;

					ex.cannonMove = sfw::getKey( CANN_LEFT ) ? tankNet::CannonMovementOptions::LEFT :
						sfw::getKey( CANN_RIGT ) ? tankNet::CannonMovementOptions::RIGHT :
						tankNet::CannonMovementOptions::HALT;

					ex.fireWish = sfw::getKey( TANK_FIRE );

					// game actions
					if ( sfw::getKey( GAME_QUIT ) ) {
						ex.msg = tankNet::TankBattleMessage::QUIT;
						break;
					}
				}
            }
			if(!playerControlled) {
				// have agent control tank
				ex = agent.update( *state, sfw::getDeltaTime() );
			}

            // begin transmission
            tankNet::send(ex);
        }
    }
    else
    {
        std::cout << "\nFailed to connect to server. Is the server online and not full?";
    }

    tankNet::term();
    sfw::termContext();

    return EXIT_SUCCESS;
}