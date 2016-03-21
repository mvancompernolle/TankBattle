#include <iostream>
#include <string>

#include <vector>

#include "dyad.h"

using std::vector;

bool isConnected = false;

static void onConnect(dyad_Event *e)
{
	printf("connected: %s\n", e->msg);
	isConnected = true;
}

static void onData(dyad_Event *e)
{
    printf("onData: ");
	printf("%s", e->data);
    isConnected = true;
}

static void onError(dyad_Event *e)
{
	printf("%s", e->msg);
}

static void onClose(dyad_Event *e)
{
	printf("%s", e->msg);
}

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




int main()
{
	dyad_init();

	dyad_Stream *s = dyad_newStream();
	dyad_addListener(s, DYAD_EVENT_CONNECT, onConnect, NULL);
	dyad_addListener(s, DYAD_EVENT_CLOSE, onClose, NULL);
	dyad_addListener(s, DYAD_EVENT_DATA, onData, NULL);
	dyad_addListener(s, DYAD_EVENT_ERROR, onError, NULL);

	dyad_connect(s, "192.168.1.16", 11000);

	while (dyad_getStreamCount() > 0)
	{
		dyad_update();

		if (isConnected)
		{
            isConnected = false;

            unsigned char msg[sizeof(tankBattleHeader)];
            tankBattleHeader ex;
            ex.messageLength = sizeof(tankBattleHeader);

            memcpy_s(msg, sizeof(tankBattleHeader), &ex, sizeof(tankBattleHeader));
            dyad_write(s, msg, sizeof(tankBattleHeader));
		}
	}

	dyad_shutdown();

	system("pause");

	return 0;
}