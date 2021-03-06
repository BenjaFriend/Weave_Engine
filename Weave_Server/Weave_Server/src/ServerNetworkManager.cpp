#include "stdafx.h"

#include "ServerNetworkManager.h"
#include "Bullet.h"

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

void ServerNetworkManager::CheckForDisconnects()
{
    std::vector < ClientProxyPtr > ClientsToDC;

    float minAllowedLastPacketFromClientTime = Timing::sInstance.GetTimef() - ClientDisconnectTimeout;

    for ( const auto& pair : EndpointToClientMap )
    {
        if ( pair.second->GetLastPacketFromClientTime() < minAllowedLastPacketFromClientTime )
        {
            ClientsToDC.push_back( pair.second );
        }
    }

    for ( ClientProxyPtr client : ClientsToDC )
    {
        HandleClientDisconnected( client );
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
    Scene.RemoveReplicatedObject( aClient->GetClientEntity() );
    LOG_TRACE( "Client removed: {}", aClient->GetName() );
}

void ServerNetworkManager::ProcessExistingClientPacket( ClientProxyPtr aClient, InputMemoryBitStream & inInputStream )
{
    UINT32 packetType;
    inInputStream.Read( packetType );

    // Keep track of the last time we received a packet from this client
    aClient->UpdateLastPacketTime();

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

        ClientProxyPtr newClient = std::make_shared< ClientProxy >(
            std::move( inFromAddress ),
            name,
            NewPlayerID++
            );

        EndpointToClientMap[ inFromAddress ] = newClient;

        // Send welcome packet to them and tell them what their ID is
        SendWelcomePacket( newClient );
        auto clientEnt = Scene.AddEntity(
            newClient->GetName(),
            NewPlayerID,
            EReplicatedClassType::ETank_Class
        );
        newClient->SetClientEntity( clientEnt );
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

    float inputMovement( 0.f );
    float inputRotation( 0.f );

    for ( size_t i = 0; i < sizeOfMoveList; ++i )
    {
        UINT8 move = 0;
        float time = 0.0f;
        inInputStream.Read( move );
        inInputStream.Read( time );

        switch ( static_cast < Input::InputType > ( move ) )
        {
        case Input::InputType::Fire:
        {
            // Get spawn pos info
            glm::vec3 bulletSpawnPoint = aClient->GetClientEntity()->GetTransform()->GetPosition();
            // Add the forward vector to the bullet spawn point
            const glm::vec3 & forward = aClient->GetClientEntity()->GetTransform()->GetForward();
            const glm::vec3 & rot = aClient->GetClientEntity()->GetTransform()->GetRotation();

            // Spawn a bullet on the server
            Entity* newBullet = Scene.AddEntity(
                "Bullet Boi",
                ++NewPlayerID,
                EReplicatedClassType::EBullet_Class
            );

            bulletSpawnPoint += ( forward * 1.5f );
            newBullet->GetTransform()->SetPosition( bulletSpawnPoint );
            newBullet->GetTransform()->SetRotation( rot );

            newBullet->AddComponent<Bullet>();
        }
        break;
        case Input::InputType::Move_Left:
        {
            LOG_TRACE( "Move left!" );
            inputRotation += 45.0f * time;
        }
        break;
        case Input::InputType::Move_Right:
        {
            LOG_TRACE( "Move_Right" );
            inputRotation -= 45.0f * time;
        }
        break;
        case Input::InputType::Move_Up:
        {
            LOG_TRACE( "Move_Up" );
            inputMovement -= 5.0f * time;
        }
        break;
        case Input::InputType::Move_Down:
        {
            LOG_TRACE( "Move_Down!" );
            inputMovement += 5.0f * time;
        }
        break;
        default:
            break;
        }
    }

    // Move the client based on their input to the server
    aClient->GetClientEntity()->GetTransform()->MoveRelative( 0.f, 0.f, inputMovement );
    aClient->GetClientEntity()->GetTransform()->Rotate( glm::vec3( 0.f, inputRotation, 0.f ) );

    Scene.SetDirtyState(
        aClient->GetClientEntity()->GetNetworkID(),
        Entity::EIEntityReplicationState::EIRS_AllState
    );

}

void ServerNetworkManager::SendWelcomePacket( ClientProxyPtr aClient )
{
    assert( aClient != nullptr );

    OutputMemoryBitStream welcPacket = {};
    welcPacket.Write( WelcomePacket );
    welcPacket.Write( aClient->GetPlayerID() );
    SendPacket( welcPacket, aClient->GetEndpoint() );
}

void ServerNetworkManager::Update( float deltaTime, float TotalTime )
{
    Scene.Update( deltaTime, TotalTime );
}

void ServerNetworkManager::UpdateAllClients()
{
    // #TODO: Make sure we only do this at a specific tick rate

    for ( auto it = EndpointToClientMap.begin(), end = EndpointToClientMap.end(); it != end; ++it )
    {
        ClientProxyPtr clientProxy = it->second;

        clientProxy->GetDeliveryNotificationManager().ProcessTimedOutPackets();

        SendStatePacket( clientProxy );
    }
}

void ServerNetworkManager::SendStatePacket( ClientProxyPtr aClient )
{
    assert( aClient != nullptr );

    OutputMemoryBitStream packet = {};
    packet.Write( StatePacket );

    // Write the delivery info needed (packet ACK id)
    aClient->GetDeliveryNotificationManager().WriteState( packet );

    // Write the number of connected clients
    packet.Write( static_cast< UINT8 >( EndpointToClientMap.size() ) );

    Scene.Write( packet, 0 );

    SendPacket( packet, aClient->GetEndpoint() );
}

void ServerNetworkManager::SendFeedMessagePacket( ClientProxyPtr aClient, const char* aMsg )
{
    assert( aClient != nullptr && aMsg != nullptr );

    OutputMemoryBitStream packet = {};
    packet.Write( FeedMessagePacket );

    packet.WriteBits( aMsg, static_cast< UINT32 >( strlen( aMsg ) ) );

    SendPacket( packet, aClient->GetEndpoint() );
}