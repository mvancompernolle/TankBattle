#pragma once

#include <vector>

#include "TankBattleHeaders.h"

namespace tankNet
{
    typedef std::vector<std::vector<int>> adjacencyMatrix;

    bool init(char * address, int port);
    bool update(double timeout);
    void term();

    void send(tankBattleHeader output);
    TankBattleStateData recieve();

    bool isConnected();     // has a connection been established
    bool isProvisioned();   // has the server provisioned this client with an ID

    adjacencyMatrix getAdjacencyMatrix();
}