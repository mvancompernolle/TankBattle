#pragma once

#include <vector>

#include "TankBattleHeaders.h"

namespace tankNet
{
    typedef std::vector<std::vector<int>> adjacencyMatrix;

    bool init(int port, char * address="127.0.0.1");        // attempts to connect to a server
    bool update(double timeout=0.0);            // how long should we wait for an update?
    void term();

    void send(TankBattleHeader output);         // send a struct containing input information
    TankBattleStateData * recieve();              // pull the last state information available

    bool isConnected();                         // has a connection been established
    bool isProvisioned();                       // has the server provisioned this client with an ID

    adjacencyMatrix getAdjacencyMatrix();

	std::ostream& operator<<(std::ostream &os, TankBattleStateData const &state);
}