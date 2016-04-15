#include "TankBattleNet.h"

#include <cassert>
#include <ostream>
#include <iostream>

#include <vector>

#include "dyad.h"

namespace tankNet
{
    static bool _isConnected;
    static bool _isProvisioned;
    static bool _isErrored;

    static dyad_Stream * stream;

    static unsigned char * lastMessage;
    static int lastMessageLength;

    void onConnect(dyad_Event *e)
    {
        _isConnected = true;
        std::cout << "Connected.\n";
    }
    void onData(dyad_Event *e)
    {
        auto msg = (TankBattleStateData*)e->data;

		// HACK: discard this transmission
		// develop a better messaging protocol
		if (msg->messageLength <= 0)
			return;

        // TODO: RTCs
        if (lastMessageLength < e->size)
        {
            delete[] lastMessage;
            lastMessageLength = e->size;
            lastMessage = new unsigned char[lastMessageLength];
        }

        memcpy_s(lastMessage, lastMessageLength, e->data, e->size);
		std::vector<unsigned char> testBytes;
		testBytes.resize(53);
		memcpy_s(testBytes.data(), 53, &msg, 53);

		for (int i = 0; i < 53; ++i)
		{
			std::cout << testBytes.data()[i] << "\n";
		}

		system("pause");

        if (!isProvisioned())
        {
            _isProvisioned = true;
            std::cout << "Provisioned.\n";
        }
    }
    void onError(dyad_Event *e)
    {
        std::cerr << "Error:" << e->msg << "\n";

        _isErrored = true;
    }
    void onClose(dyad_Event *e)
    {
        printf("Stream Close: ");
        printf("%s", e->msg);

        _isConnected = false;
        _isProvisioned = false;
    }
    
    bool init(int port, char * address)
    {
        dyad_init();

        assert(stream == nullptr);

        lastMessageLength = sizeof(TankBattleStateData);
        lastMessage = new unsigned char[lastMessageLength];

        stream = dyad_newStream();

        dyad_addListener(stream, DYAD_EVENT_DATA, onData, 0);
        dyad_addListener(stream, DYAD_EVENT_ERROR, onError, 0);
        dyad_addListener(stream, DYAD_EVENT_CONNECT, onConnect, 0);
        dyad_addListener(stream, DYAD_EVENT_CLOSE, onClose, 0);


        if (dyad_connect(stream, address, port) == -1)
        {
            std::cerr << "Failed to resolve server host.\n";
            return false;
        }
    }

    bool update(double timeout)
    {
        if (_isErrored)
            return false;

        dyad_setUpdateTimeout(timeout);
        dyad_update();

        return true;
    }

    void term()
    {
        _isConnected = false;
        _isProvisioned = false;

        if (stream != nullptr)
        {
            dyad_close(stream);
        }

        delete[] lastMessage;

        dyad_shutdown();
    }

    void send(TankBattleCommand output)
    {
        if (!isProvisioned)
            return;

        const int msgSize = sizeof(TankBattleCommand);
        unsigned char msg[msgSize];

        memcpy_s(&msg, msgSize, &output, sizeof(TankBattleCommand));
        dyad_write(stream, &msg, msgSize);
    }

    TankBattleStateData * recieve()
    {
        TankBattleStateData * lastState = ((TankBattleStateData*)lastMessage);
        //lastState->tacticoolData = (TankTacticoolInfo*)(((char*)lastState) + offsetof(TankBattleStateData, tacticoolData));
		lastState->tacticoolData = (TankTacticoolInfo*)(((char*)lastState) + TankBattleStateData::TACTICOOL_ARRAY);
        return lastState;
    }

    bool isConnected()
    {
        return _isConnected;
    }

    bool isProvisioned()
    {
        return _isProvisioned;
    }

    adjacencyMatrix getAdjacencyMatrix()
    {
        auto adjMat = adjacencyMatrix();
        adjMat.reserve(10);

        for (int i = 0; i < adjMat.size(); ++i)
        {
            adjMat[i].reserve(10);
        }

        for (int i = 0; i < adjMat.size(); ++i)
        {
            for (int j = 0; i < adjMat[i].size(); ++j)
            {
                adjMat[i][j] = 1;
            }
        }

        return adjMat;
    }

	std::ostream& operator<<(std::ostream &os, TankBattleStateData const &state)
	{
		os << "Player ID: " << state.playerID << "\n";
		os << "Health: " << state.currentHealth << "\n";
		for (int i = 0; i < 3; ++i)
		{
			os << "Position " << i << " = " << state.position[i] << "\n";
		}
		for (int i = 0; i < 3; ++i)
		{
			os << "Forward " << i << " = " << state.forward[i] << "\n";
		}
		for (int i = 0; i < 3; ++i)
		{
			os << "Cannon Forward " << i << " = " << state.cannonForward[i] << "\n";
		}
		os << "Can Fire: " << state.canFire << "\n";
		os << "Tacticool Count: " << state.tacticoolCount << "\n";

		return os;
	}
}