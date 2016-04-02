#include <iostream>

enum tankBattleMessage
{
    NONE,
    LOGIN,
    QUIT
};

#include "sfwdraw.h"

const char FWRD_KEY = 'w';
const char REVS_KEY = 's';
const char LEFT_KEY = 'a';
const char RIGT_KEY = 'd';

const char FIRE_KEY = 'e';

#include "dyad.h"

int myPlayerID = 0;



enum Movement
{
    HALT,
    FWRD,
    BACK,
    LEFT,
    RIGHT,
};

struct tankBattleHeader
{
    int playerID = -1;
    tankBattleMessage msg = QUIT;
    Movement move = HALT;
    int messageLength;  // this is only if we had like, a payload to deliver.
};

bool isConnected = false;

static void onConnect(dyad_Event *e)
{
    printf("connected: %s\n", e->msg);
    isConnected = true;
}
static void onData(dyad_Event *e)
{
    printf("onData: ");
    //printf("%s", e->data);

    auto msg = (tankBattleHeader*)e->data;
    std::cout << "DIAGNOSTIC STATUS:\n";
    std::cout << (msg->playerID) << "\n";

    myPlayerID = msg->playerID;
}
static void onError(dyad_Event *e)
{
    printf("onError: ");
    printf("%s", e->msg);
}
static void onClose(dyad_Event *e)
{
    printf("onClose: ");
    printf("%s", e->msg);
}

#define TANK_FWRD 'W'
#define TANK_BACK 'S'

bool inputPressed()
{
    for (unsigned int i = 0; i < 150; ++i)
    {
        if (sfw::getKey(i))
            return true;
    }

    return false;
}

int main()
{
    dyad_init();

    dyad_Stream *s = dyad_newStream();
    dyad_addListener(s, DYAD_EVENT_CONNECT, onConnect, NULL);
    dyad_addListener(s, DYAD_EVENT_CLOSE, onClose, NULL);
    dyad_addListener(s, DYAD_EVENT_DATA, onData, NULL);
    dyad_addListener(s, DYAD_EVENT_ERROR, onError, NULL);

    dyad_connect(s, "127.0.0.1", 11000);

    sfw::initContext(400, 400, "TankController");

    while (dyad_getStreamCount() > 0 && sfw::stepContext())
    {
        // check TCP streams via dyad
        dyad_setUpdateTimeout(0);
        dyad_update();

        if (isConnected)
        {
            std::cout << "poll.\n";

            // poll for input
            if (inputPressed())
            {
                std::cout << "Send.\n";

                // prepare message
                const int msgSize = sizeof(tankBattleHeader);
                unsigned char msg[msgSize];

                tankBattleHeader ex;
                ex.messageLength = msgSize;    // TODO: support for dynamic message length
                ex.playerID = myPlayerID;

                if (sfw::getKey(TANK_FWRD))
                {
                    ex.move = FWRD;
                }
                else if (sfw::getKey(TANK_BACK))
                {
                    ex.move = BACK;
                }
                else
                {
                    ex.move = HALT;
                }

                // begin transmission
                memcpy_s(&msg, msgSize, &ex, sizeof(tankBattleHeader));
                dyad_write(s, &msg, msgSize);
            }
        }

        
    }

    dyad_close(s);
    dyad_shutdown();

    sfw::termContext();

    return 0;
}