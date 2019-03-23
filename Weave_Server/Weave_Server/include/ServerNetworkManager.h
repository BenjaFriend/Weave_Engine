#pragma once

#include "Networking/NetworkManager.h"
#include "ClientProxy.h"
#include <map>

class ServerNetworkManager : public NetworkManager
{
public:

    ServerNetworkManager();

    virtual ~ServerNetworkManager();


protected:

    virtual void ProcessPacket( InputMemoryBitStream& inInputStream, const boost::asio::ip::udp::endpoint & inFromAddress ) override;

private:

    // Handle new client packet
    void ProcessExistingClientPacket( ClientProxyPtr aClient, InputMemoryBitStream& inInputStream );

    /// <summary>
    /// Handle a packet that was received from an endpoint that does not exist
    /// in our current client map
    /// </summary>
    /// <param name="inInputStream">Data from the new client</param>
    /// <param name="inFromAddress">Endpoint of the client</param>
    void ProcessNewClientPacket( InputMemoryBitStream& inInputStream, const boost::asio::ip::udp::endpoint & inFromAddress );

    /// <summary>
    /// Send a welcome packet to the given client
    /// </summary>
    /// <param name="aClient">The client to send the welcome packet to</param>
    void SendWelcomePacket( ClientProxyPtr aClient );

    /// <summary>
    /// Send the state packet to the given  client
    /// </summary>
    /// <param name="aClient">The client to update</param>
    void SendStatePacket( ClientProxyPtr aClient );

    /// <summary>
    /// Update all clients that are connected with the proper state data
    /// </summary>
    void UpdateAllClients();
    
    UINT32 NewPlayerID;

    /** Map of endpoints to clients */
    typedef std::map< boost::asio::ip::udp::endpoint, ClientProxyPtr >	AddressToClientMap;

    AddressToClientMap EndpointToClientMap;
};