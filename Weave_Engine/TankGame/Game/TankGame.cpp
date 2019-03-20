#include "TankGame.h"

using namespace Tanks;

TankGame::TankGame( HINSTANCE hInstance )
    : Game( hInstance )
{

}

TankGame::~TankGame()
{

}

void TankGame::Init()
{
    Game::Init();
}

void TankGame::Update( float deltaTime, float totalTime )
{
    Game::Update( deltaTime, totalTime );
}

void TankGame::Quit()
{
    Game::Quit();
}