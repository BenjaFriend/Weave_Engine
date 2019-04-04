#pragma once

#include "Networking/NetworkManager.h"
#include "ClientProxy.h"
#include "Input/InputBindings.h"
#include "ServerScene.h"

#include <map>

class ServerNetworkManager : public NetworkManager
{
public:

    ServerNetworkManager( std::shared_ptr< boost::asio::io_service > aServce );

    virtual ~ServerNetworkManager();

    /// <summary>
    /// Update all clients that are connected with the proper state data
    /// </summary>
    void UpdateAllClients();

    /// <summary>
    /// Handle a client resetting their connection to the server
    /// </summary>
    /// <param name="inFromAddress">the adddress to handle</param>
    virtual void HandleConnectionReset( const boost::asio::ip::udp::endpoint & inFromAddress ) override;

    /// <summary>
    /// Check to see if there has been any client that should be disconnected
    /// from this server, if so, remove them
    /// </summary>
    void CheckForDisconnects();

protected:

    virtual void ProcessPacket( InputMemoryBitStream& inInputStream, const boost::asio::ip::udp::endpoint & inFromAddress ) override;

private:

    /// <summary>
    /// Handle a client leaving the server
    /// </summary>
    /// <param name="aClient">The client which has DC'd</param>
    void HandleClientDisconnected( ClientProxyPtr aClient );

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
    /// Process a packet filled with a move list from the client
    /// </summary>
    /// <param name="aClient"></param>
    /// <param name="inInputStream"></param>
    void ProcessInputPacket( ClientProxyPtr aClient, InputMemoryBitStream& inInputStream );

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
    /// Send a feed update message to a client. This is typcially just an update message
    /// like "Player has jonied the lobby" or some other string message to show up in the 
    /// client's feed
    /// </summary>
    /// <param name="aClient">The client to update</param>
    void SendFeedMessagePacket( ClientProxyPtr aClient, const char* aMsg );

    /** The ID count to give to each player */
    UINT32 NewPlayerID = 0;

    /** The amount of time it takes for a client to be removed from the server
    after not receiving a packet from them*/
    float ClientDisconnectTimeout = 5.f;

    /** The current scene that is loaded */
    ServerScene Scene;

    /** Map of endpoints to clients */
    typedef std::map< boost::asio::ip::udp::endpoint, ClientProxyPtr >	AddressToClientMap;
    AddressToClientMap EndpointToClientMap;
};