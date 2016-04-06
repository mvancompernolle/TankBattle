#include "TankBattleNet.h"

#include <cassert>
#include <iostream>

#include "dyad.h"

namespace tankNet
{
    static bool _isConnected;
    static bool _isProvisioned;
    static bool _isErrored;

    static dyad_Stream * stream;

    static TankBattleStateData lastState;

    void onConnect(dyad_Event *e)
    {
        _isConnected = true;
        std::cout << "Connected.\n";
    }
    void onData(dyad_Event *e)
    {
        auto msg = (TankBattleStateData*)e->data;
        //assert(e->size == sizeof(TankBattleStateData));

        // TODO: RTCs

        // is this a game state packet?
        if (isProvisioned())
        {
            lastState = *msg;
        }
        else // or is this for setting first join?
        {
            lastState = *msg;
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
    
    bool init(char * address, int port)
    {
        dyad_init();

        assert(stream == nullptr);

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

        dyad_shutdown();
    }

    void send(tankBattleHeader output)
    {
        if (!isProvisioned)
            return;

        const int msgSize = sizeof(tankBattleHeader);
        unsigned char msg[msgSize];

        memcpy_s(&msg, msgSize, &output, sizeof(tankBattleHeader));
        dyad_write(stream, &msg, msgSize);
    }

    TankBattleStateData recieve()
    {
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

}