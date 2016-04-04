#include <iostream>
#include <random>
#include <time.h>

enum tankBattleMessage
{
    NONE,
    JOIN,
    GAME,
    QUIT
};

#include "sfwdraw.h"

#undef NONE

const char FWRD_KEY = 'w';
const char REVS_KEY = 's';
const char LEFT_KEY = 'a';
const char RIGT_KEY = 'd';

const char FIRE_KEY = 'e';

#include "dyad.h"

int myPlayerID = -1;
int myUUID = -1;


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
    int uuid = -1;

    int playerID = -1;
    tankBattleMessage msg = QUIT;
    Movement move = HALT;
    int messageLength;  // this is only if we had like, a payload to deliver.
};

bool isConnected = false;
bool isWaiting = false;
bool isProvisioned = false;

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

    isProvisioned = true;
    isWaiting = false;
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

    isConnected = false;
}
static int getUUID()
{
    return rand() % 100000000;  // TODO: make a better algo for this
}

#define TANK_FWRD 'W'
#define TANK_BACK 'S'

#define TANK_LEFT 'A'
#define TANK_RIGT 'D'

#define GAME_QUIT 'Q'

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
    srand(time(NULL));
    myUUID = getUUID();

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
        dyad_setUpdateTimeout(0.0);
        dyad_update();

        if (isConnected && !isWaiting)
        {
            // prepare message
            const int msgSize = sizeof(tankBattleHeader);
            unsigned char msg[msgSize];

            tankBattleHeader ex;
            ex.msg = tankBattleMessage::NONE;
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
                    ex.move = FWRD;
                }
                else if (sfw::getKey(TANK_BACK))
                {
                    ex.move = BACK;
                }
                else if (sfw::getKey(TANK_LEFT))
                {
                    ex.move = LEFT;
                }
                else if (sfw::getKey(TANK_RIGT))
                {
                    ex.move = RIGHT;
                }
                else
                {
                    ex.move = HALT;
                }

                // game actions
                if (sfw::getKey(GAME_QUIT))
                {
                    ex.msg = QUIT;
                }
            }

            if (isConnected)
            {
                // begin transmission
                memcpy_s(&msg, msgSize, &ex, sizeof(tankBattleHeader));
                dyad_write(s, &msg, msgSize);
            }
        }
    }

    dyad_close(s);
    dyad_shutdown();

    sfw::termContext();

    system("pause");

    return 0;
}