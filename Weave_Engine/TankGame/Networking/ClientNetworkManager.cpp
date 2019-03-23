#include "pch.h"
#include "ClientNetworkManager.h"

using namespace Tanks;

// Static initializations 
ClientNetworkManager* ClientNetworkManager::Instance = nullptr;


ClientNetworkManager::ClientNetworkManager( const char * aServerAddr, const unsigned short aPort, const std::string& aName )
    : NetworkManager()
{
    ServerEndpoint =
        boost::asio::ip::udp::endpoint(
            boost::asio::ip::address::from_string( aServerAddr ),
            aPort
        );

    ClientState = EClientState::SayingHello;

    Name = aName;
}

ClientNetworkManager::~ClientNetworkManager()
{

}

ClientNetworkManager* Tanks::ClientNetworkManager::StaticInit( const char * aServerAddr, const unsigned short aPort, const std::string& aName )
{
    assert( Instance == nullptr );

    Instance = new ClientNetworkManager( aServerAddr, aPort, aName );
    Instance->Init( 50000 );

    LOG_TRACE( "" );

    return Instance;
}

void Tanks::ClientNetworkManager::ReleaseInstance()
{
    SAFE_DELETE( Instance );
}

void Tanks::ClientNetworkManager::SendOutgoingPackets()
{
    // An example of how you can send a packet to a specific spot


    switch ( ClientState )
    {
    case Tanks::ClientNetworkManager::EClientState::SayingHello:
    {
        // Create a test packet
        OutputMemoryBitStream welcomePacket;
        UINT32 packetType = 'HELO';

        welcomePacket.Write( packetType );
        LOG_TRACE( "Send packet: {}", packetType );
        // Send it
        SendPacket( welcomePacket, ServerEndpoint );
    }
    break;

    case Tanks::ClientNetworkManager::EClientState::Welcomed:
    {
        
    }
    break;
    default:
        break;
    }
}

void ClientNetworkManager::ProcessPacket( InputMemoryBitStream& inInputStream, const boost::asio::ip::udp::endpoint & inFromAddress )
{
    LOG_TRACE( "Client process the backet booiys!" );
}