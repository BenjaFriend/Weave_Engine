#include "pch.h"
#include "ClientNetworkManager.h"
#include "Scenes/SceneManager.h"

using namespace Tanks;

// Static initializations 
ClientNetworkManager* ClientNetworkManager::Instance = nullptr;


ClientNetworkManager::ClientNetworkManager(
    std::shared_ptr< boost::asio::io_service > aService,
    const char * aServerAddr, const unsigned short aPort, 
    const std::string& aName )
    : NetworkManager( aService ), Name( aName ), NotifManager( true, false )
{
    ServerEndpoint =
        boost::asio::ip::udp::endpoint(
            boost::asio::ip::address::from_string( aServerAddr ),
            aPort
        );

    ClientState = EClientState::SayingHello;

    TimeOfLastStatePacket = Timing::sInstance.GetTimef();
}

ClientNetworkManager::~ClientNetworkManager()
{

}

ClientNetworkManager* Tanks::ClientNetworkManager::StaticInit(
    std::shared_ptr< boost::asio::io_service > aService,
    const char * aServerAddr,
    const unsigned short aPort,
    const std::string& aName )
{
    // The client should be able to reconnect to a different server
    ReleaseInstance();

    Instance = new ClientNetworkManager( aService, aServerAddr, aPort, aName );
    Instance->Init( 50000 );

    LOG_TRACE( "Client initalized! " );

    return Instance;
}

void Tanks::ClientNetworkManager::ReleaseInstance()
{
    SAFE_DELETE( Instance );
}

void Tanks::ClientNetworkManager::SendOutgoingPackets( float totalTime )
{
    switch ( ClientState )
    {
    case Tanks::ClientNetworkManager::EClientState::SayingHello:
    {
        // Only send this packet if the time between the packet sends is viable
        if ( totalTime > TimeOfLastHello + TimeBetweenHellos )
        {
            SendHelloPacket();
            TimeOfLastHello = totalTime;
        }
    }
    break;

    // We have been welcomed into the game and we can start sending our input updates
    case Tanks::ClientNetworkManager::EClientState::Welcomed:
    {
        // Send input state to the server
        if ( totalTime > TimeOfLastInputUpdate + TimeBetweenInputUpdate )
        {
            SendInputPacket();
            TimeOfLastInputUpdate = totalTime;
        }
        // Send heartbeat packet to the server
        if ( totalTime > TimeOfLastHeartbeat + TimeBetweenHeartBeats )
        {
            SendHeartbeatPacket();
            TimeOfLastHeartbeat = totalTime;
        }
    }
    break;

    default:
        break;
    }

    // Check if we should disconnect
    if ( totalTime > TimeOfLastStatePacket + TimeUntilTimeout )
    {
        LOG_WARN( "We should disconnect from the server!" );
    }
}

void ClientNetworkManager::ProcessPacket( InputMemoryBitStream& inInputStream, const boost::asio::ip::udp::endpoint & inFromAddress )
{
    UINT32 packetType;
    inInputStream.Read( packetType );

    switch ( packetType )
    {
    case  WelcomePacket:
    {
        // The server has welcomed us! Weee
        ClientState = ClientNetworkManager::EClientState::Welcomed;
        // Get our player ID
        inInputStream.Read( PlayerID );

        LOG_TRACE( "We have  been welcomed! Our ID is {}", PlayerID );
    }
    break;
    case  StatePacket:
    {
        //  As long as we have been welcomed by the server
        if ( NotifManager.ReadAndProcessState(inInputStream) )
        {
            // Update our local  world based on this new info
            ProcessStatePacket( inInputStream );
        }

        //TimeOfLastStatePacket = TotalTime;
    }
    break;
    default:
        break;
    }

}

void Tanks::ClientNetworkManager::SendHelloPacket()
{
    // Create a test packet
    OutputMemoryBitStream welcomePacket;
    welcomePacket.Write( HelloPacket );
    welcomePacket.Write( Name );

    // Send it
    SendPacket( welcomePacket, ServerEndpoint );

    LOG_TRACE( "Sent hello packet!" );
}

void Tanks::ClientNetworkManager::SendInputPacket()
{
    if ( PlayerMoves::Instance->HasMoves() )
    {
        // If the client has hit any buttons
        OutputMemoryBitStream output = {};
        output.Write( InputPacket );

        // For every move in the queue
        const std::deque<Input::InputType> & moveQueue = PlayerMoves::Instance->GetMoveQueue();

        // Write the size of how many moves there are
        output.Write( static_cast< UINT32 > ( moveQueue.size() ) );
        for ( const auto & move : moveQueue )
        {
            output.Write( static_cast< UINT8 > ( move ) );
        }

        SendPacket( output, ServerEndpoint );

        // Clear our move list, as we have sent everything in it
        PlayerMoves::Instance->Clear();
    }
}

void Tanks::ClientNetworkManager::SendHeartbeatPacket()
{
    // Create a test packet
    OutputMemoryBitStream beatPacket;
    beatPacket.Write( HeartbeatPacket );

    // Send it
    SendPacket( beatPacket, ServerEndpoint );
}

void Tanks::ClientNetworkManager::ProcessStatePacket( InputMemoryBitStream & inInputStream )
{
    TimeOfLastStatePacket = Timing::sInstance.GetTimef();

    // Keep track of how many connected players there are on the client
    UINT8 playerCount = 0;
    inInputStream.Read( playerCount );
    NumConnectedPlayers = playerCount;

    // Read in the state of the scene
    using namespace SceneManagement;
    Scene* scene = SceneManager::GetInstance()->GetActiveScene();
    scene->Read( inInputStream );
}
