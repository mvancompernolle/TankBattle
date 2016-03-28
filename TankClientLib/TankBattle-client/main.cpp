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

	dyad_connect(s, "127.0.0.1", 11000);

	while (dyad_getStreamCount() > 0)
	{
        // check TCP streams via dyad
		dyad_update();

        if (isConnected)
        {
            // poll for input
            if (_kbhit())
            {
                std::cout << "Send.\n";

                // prepare message
                int msgSize = sizeof(tankBattleHeader) + 12;
                std::vector<unsigned char> msg;
                msg.reserve(msgSize * 2);
                tankBattleHeader ex;
                ex.messageLength = msgSize;    // TODO: support for dynamic message length

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

                //std::copy()

                // begin transmission
                //memcpy_s(&msg, msgSize, &ex, sizeof(tankBattleHeader));
                dyad_write(s, &msg, msgSize);
            }
		}
	}

    dyad_close(s);
	dyad_shutdown();

	system("pause");

	return 0;
}