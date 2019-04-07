#include "stdafx.h"

#include "ServerNetworkManager.h"

ServerNetworkManager::ServerNetworkManager( std::shared_ptr< boost::asio::io_service > aServce )
    : NetworkManager( aServce )
{
    // Create a scene to replicate

}

ServerNetworkManager::~ServerNetworkManager()
{
    LOG_TRACE( "--- ServerNetworkManage DTOR ---" );
}

void ServerNetworkManager::HandleConnectionReset( const boost::asio::ip::udp::endpoint & inFromAddress )
{
    auto it = EndpointToClientMap.find( inFromAddress );
    if ( it != EndpointToClientMap.end() )
    {
        HandleClientDisconnected( it->second );
    }
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

void ServerNetworkManager::HandleClientDisconnected( ClientProxyPtr aClient )
{
    // Tell the scene that they have left and we should remove their
    // game object!
    EndpointToClientMap.erase( aClient->GetEndpoint() );
    Scene.RemoveReplicatedObject( aClient->GetClientEntity().get() );
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
    // TODO: Handle heartbeat
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

        newClient->SetClientEntity( Scene.AddEntity( newClient->GetName() ) );
        newClient->GetClientEntity()->SetNetworkID( newClient->GetPlayerID() );
        Scene.AddReplicatedObject( newClient->GetClientEntity().get() );
    }
    else
    {
        // Bad boi, this isn't what we want from a new client! 
        // Possibly malicious
    }
}

void ServerNetworkManager::ProcessInputPacket(ClientProxyPtr aClient, InputMemoryBitStream & inInputStream)
{
	UINT32 sizeOfMoveList = 0;
	inInputStream.Read(sizeOfMoveList);

	float inputMovement(0.f);
	float inputRotation(0.f);

	for (size_t i = 0; i < sizeOfMoveList; ++i)
	{
		UINT8 move = 0;
		inInputStream.Read(move);

		switch (static_cast <Input::InputType> (move))
		{
		case Input::InputType::Fire:
		{
			LOG_TRACE("PLAYER FIRE MOVE!");
		}
		break;
		case Input::InputType::Move_Left:
		{
			LOG_TRACE("Move left!");
			inputRotation += 5.0f;
		}
		break;
		case Input::InputType::Move_Right:
		{
			LOG_TRACE("Move_Right");
			inputRotation -= 5.0f;
		}
		break;
		case Input::InputType::Move_Up:
		{
			LOG_TRACE("Move_Up");
			inputMovement -= 0.25f;
		}
		break;
		case Input::InputType::Move_Down:
		{
			LOG_TRACE("Move_Down!");
			inputMovement += 0.25f;
		}
		break;
		default:
			break;
		}
	}

	// Move the client based on their input to the server
	aClient->GetClientEntity()->GetTransform()->MoveRelative(0, 0, inputMovement);
	aClient->GetClientEntity()->GetTransform()->Rotate(glm::vec3(0, inputRotation, 0));
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

void ServerNetworkManager::SendFeedMessagePacket( ClientProxyPtr aClient, const char* aMsg )
{
    assert( aMsg != nullptr );
    
    OutputMemoryBitStream packet = {};
    packet.Write( FeedMessagePacket );

    packet.WriteBits( aMsg, static_cast< UINT32 >( strlen( aMsg ) ) );

    SendPacket( packet, aClient->GetEndpoint() );
}