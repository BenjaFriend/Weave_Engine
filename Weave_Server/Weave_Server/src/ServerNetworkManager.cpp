#include "stdafx.h"

#include "ServerNetworkManager.h"

ServerNetworkManager::ServerNetworkManager()
    : NetworkManager()
{

}

ServerNetworkManager::~ServerNetworkManager()
{
    LOG_TRACE( "--- ServerNetworkManage DTOR ---" );
}

void ServerNetworkManager::ProcessPacket( InputMemoryBitStream& inInputStream, const boost::asio::ip::udp::endpoint & inFromAddress )
{
    auto it = EndpointToClientMap.find( inFromAddress );

    if ( it == EndpointToClientMap.end() )
    {
        //didn't find one? it's a new client..is the a HELO? if so, create a client proxy...
        ProcessNewClientPacket( inInputStream, inFromAddress );
    }
    else
    {
        ProcessExistingClientPacket( ( *it ).second, inInputStream );
    }
}

void ServerNetworkManager::ProcessExistingClientPacket( ClientProxyPtr aClient, InputMemoryBitStream & inInputStream )
{
    UINT32 packetType;
    inInputStream.Read( packetType );

    switch ( packetType )
    {
    case HelloPacket:
    {
        SendWelcomePacket( aClient );
    }
    break;
    case InputPacket:
    {
        // Handle the input given from the player
        LOG_TRACE( "Received INPUT packet!!" );
        // Update this players input
    }
    break;

    default:
        break;
    }

}

void ServerNetworkManager::ProcessNewClientPacket( InputMemoryBitStream & inInputStream, const boost::asio::ip::udp::endpoint & inFromAddress )
{
    // Send them a welcome packet

    UINT32 packetType;
    inInputStream.Read( packetType );

    if ( packetType == HelloPacket )
    {
        std::string name;
        inInputStream.Read( name );

        ClientProxyPtr newClient = std::make_shared< ClientProxy >( std::move( inFromAddress ), name, NewPlayerID++ );

        EndpointToClientMap [ inFromAddress ] = newClient;

        // Send welcome packet to them and tell them what their ID is
        SendWelcomePacket( newClient );
    }
    else
    {
        // Bad boi, this isn't what we want from a new client! 
        // Possibly malicious
    }

}

void ServerNetworkManager::SendWelcomePacket( ClientProxyPtr aClient )
{
    OutputMemoryBitStream welcPacket = {};
    welcPacket.Write( WelcomePacket );
    welcPacket.Write( aClient->GetPlayerID() );
    SendPacket( welcPacket, aClient->GetEndpoint() );
    LOG_TRACE( "Sent  welcome packet to client {} ID {}", aClient->GetName(), aClient->GetPlayerID() );
}

void ServerNetworkManager::SendStatePacket( ClientProxyPtr aClient )
{
    OutputMemoryBitStream packet = {};
    packet.Write( StatePacket );

    // #TODO Write the scene data to this state packet

    SendPacket( packet, aClient->GetEndpoint() );
}

void ServerNetworkManager::UpdateAllClients()
{
    // #TODO: Make sure we only do this at a specific tick rate

    for ( auto it = EndpointToClientMap.begin(), end = EndpointToClientMap.end(); it != end; ++it )
    {
        SendStatePacket( it->second );
    }

}