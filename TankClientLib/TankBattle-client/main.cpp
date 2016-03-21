#include <iostream>
#include <string>

#include "dyad.h"

bool isConnected = false;

static void onConnect(dyad_Event *e)
{
	printf("connected: %s\n", e->msg);
	isConnected = true;
}

static void onData(dyad_Event *e)
{
	printf("%s", e->data);
}

static void onError(dyad_Event *e)
{
	printf("%s", e->msg);
}

static void onClose(dyad_Event *e)
{
	printf("%s", e->msg);
}

int main()
{
	dyad_init();
	//	dyad_setUpdateTimeout(0.0);

	dyad_Stream *s = dyad_newStream();
	dyad_addListener(s, DYAD_EVENT_CONNECT, onConnect, NULL);
	dyad_addListener(s, DYAD_EVENT_CLOSE, onClose, NULL);
	dyad_addListener(s, DYAD_EVENT_DATA, onData, NULL);
	dyad_addListener(s, DYAD_EVENT_ERROR, onError, NULL);

	dyad_connect(s, "10.15.22.28", 11000);

	while (dyad_getStreamCount() > 0)
	{
		dyad_update();

		if (isConnected)
		{
			isConnected = false;
			dyad_writef(s, "Hello World");
		}
	}

	dyad_shutdown();

	system("pause");

	return 0;
}