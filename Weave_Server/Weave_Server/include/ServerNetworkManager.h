#pragma once

#include "Networking/NetworkManager.h"

class ServerNetworkManager : public NetworkManager
{
public:

    ServerNetworkManager();

    virtual ~ServerNetworkManager();


protected:

    virtual void ProcessPacket() override;

private:

    // Handle new client packet

    // Update all clients method
    void UpdateAllClients();

    // A boost socket to send data back on
};