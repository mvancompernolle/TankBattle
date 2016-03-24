#include <iostream>
#include <string>
#include <vector>

#include <conio.h>

#include "dyad.h"

using std::vector;

enum tankBattleMessage
{
    NONE,
    FWRD,
    BACK,
    KILL,
    QUIT
};

struct tankBattleHeader
{
    int playerID = -1;
    tankBattleMessage msg = QUIT;
    int messageLength;
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

#define TANK_FWRD 'w'
#define TANK_BACK 's'

int myPlayerID = 0;

int main()
{
	dyad_init();

	dyad_Stream *s = dyad_newStream();
	dyad_addListener(s, DYAD_EVENT_CONNECT, onConnect, NULL);
	dyad_addListener(s, DYAD_EVENT_CLOSE, onClose, NULL);
	dyad_addListener(s, DYAD_EVENT_DATA, onData, NULL);
	dyad_addListener(s, DYAD_EVENT_ERROR, onError, NULL);

	dyad_connect(s, "10.15.22.100", 11000);

	while (dyad_getStreamCount() > 0)
	{
        // check TCP streams via dyad
		dyad_update();

        if (isConnected)
        {
            // poll for input
            if (_kbhit())
            {
                // prepare message
                unsigned char msg[sizeof(tankBattleHeader)];
                tankBattleHeader ex;
                ex.messageLength = sizeof(tankBattleHeader);    // TODO: support for dynamic message length

                char input = _getch();

                switch (input)
                {
                case TANK_FWRD:
                    ex.msg = FWRD;
                    break;
                case TANK_BACK:
                    ex.msg = BACK;
                    break;
                default:
                    ex.msg = NONE;
                    break;
                }

                // begin transmission
                memcpy_s(msg, sizeof(tankBattleHeader), &ex, sizeof(tankBattleHeader));
                dyad_write(s, msg, sizeof(tankBattleHeader));
            }
		}
	}

	dyad_shutdown();

	system("pause");

	return 0;
}