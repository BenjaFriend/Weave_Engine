#include "TankGame.h"

using namespace Tanks;

TankGame::TankGame( HINSTANCE hInstance )
    : Game( hInstance )
{
    NetMan = new NetworkManager();
    
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
}

void TankGame::Quit()
{
    Game::Quit();
}