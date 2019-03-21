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
    NetMan->Init( 50000 );
}

void TankGame::Update( float deltaTime, float totalTime )
{
    Game::Update( deltaTime, totalTime );

    NetMan->ProcessIncomingPackets();
}

void TankGame::Quit()
{
    Game::Quit();
}