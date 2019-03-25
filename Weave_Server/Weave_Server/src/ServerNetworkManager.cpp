#include "stdafx.h"

#include "ServerNetworkManager.h"

ServerNetworkManager::ServerNetworkManager()
    : NetworkManager()
{
    // Create a scene to replicate

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
        ProcessInputPacket( aClient, inInputStream );
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

void ServerNetworkManager::ProcessInputPacket( ClientProxyPtr aClient, InputMemoryBitStream & inInputStream )
{
    UINT32 sizeOfMoveList = 0;
    inInputStream.Read( sizeOfMoveList );

    for ( size_t i = 0; i < sizeOfMoveList; ++i )
    {
        UINT8 move = 0;
        inInputStream.Read( move );
        switch ( static_cast < Input::InputType > ( move ) )
        {
        case Input::InputType::Fire:
        {
            LOG_TRACE( "PLAYER FIRE MOVE!" );
        }
        break;
        case Input::InputType::Move_Left:
        {
            LOG_TRACE( "Move left!" );
        }
        break;
        default:
            break;
        }
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

void ServerNetworkManager::UpdateAllClients()
{
    // #TODO: Make sure we only do this at a specific tick rate

    for ( auto it = EndpointToClientMap.begin(), end = EndpointToClientMap.end(); it != end; ++it )
    {
        SendStatePacket( it->second );
    }
}

void ServerNetworkManager::SendStatePacket( ClientProxyPtr aClient )
{
    OutputMemoryBitStream packet = {};
    packet.Write( StatePacket );

    // #TODO Write the scene data to this state packet
    // Write the number of connected clients
    packet.Write( static_cast< UINT8 >( EndpointToClientMap.size() ) );

    Scene.Write( packet, 0 );

    SendPacket( packet, aClient->GetEndpoint() );
}