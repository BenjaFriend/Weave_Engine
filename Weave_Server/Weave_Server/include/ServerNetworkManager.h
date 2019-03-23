#pragma once

#include "Networking/NetworkManager.h"

class ServerNetworkManager : public NetworkManager
{
public:

    ServerNetworkManager();

    virtual ~ServerNetworkManager();


protected:

    virtual void ProcessPacket( InputMemoryBitStream& inInputStream, const boost::asio::ip::udp::endpoint & inFromAddress ) override;

private:

    // Handle new client packet

    // Update all clients method
    void UpdateAllClients();

    // A boost socket to send data back on
};