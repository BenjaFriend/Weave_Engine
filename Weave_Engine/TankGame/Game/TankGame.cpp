#include "pch.h"
#include "TankGame.h"

using namespace Tanks;

TankGame::TankGame( HINSTANCE hInstance )
    : Game( hInstance )
{
    NetMan = new ClientNetworkManager();
    
}

TankGame::~TankGame()
{
    SAFE_DELETE( NetMan );
}

void TankGame::Init()
{
    Game::Init();
    // Listen for incoming data on port 50000
    NetMan->Init( 50000 );
}

void TankGame::Update( float deltaTime, float totalTime )
{
    Game::Update( deltaTime, totalTime );

    NetMan->ProcessIncomingPackets();

    // Create a test packet
    OutputMemoryBitStream welcomePacket;
    welcomePacket.Write( 'HELO' );
    
    // To localhost, port 50001
    boost::asio::ip::udp::endpoint target( 
        boost::asio::ip::address::from_string( "127.0.0.1" ),
        50001 
    );

    // Send it
    NetMan->SendPacket( welcomePacket, target );
}

void TankGame::Quit()
{
    Game::Quit();
}