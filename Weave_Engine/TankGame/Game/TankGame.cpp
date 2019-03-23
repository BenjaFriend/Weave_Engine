#include "pch.h"
#include "TankGame.h"

using namespace Tanks;

TankGame::TankGame( HINSTANCE hInstance )
    : Game( hInstance )
{
}

TankGame::~TankGame()
{
    // Release network manager instance
    ClientNetworkManager::ReleaseInstance();
}

void TankGame::Init()
{
    Game::Init();
}

void TankGame::Update( float deltaTime, float totalTime )
{
    Game::Update( deltaTime, totalTime );

    switch ( GameState )
    {
    case Tanks::EGameState::Playing:
    {
        NetMan->ProcessIncomingPackets();
        NetMan->SendOutgoingPackets();
    }
    break;
    default:
        break;
    }
}

void TankGame::Quit()
{
    GameState = EGameState::Quitting;

    // Tell the server that we quit

    Game::Quit();
}

void Tanks::TankGame::DrawUI()
{
    Game::DrawUI();

    switch ( GameState )
    {
    case Tanks::EGameState::MainMenu:
        DrawMainMenu();
        break;
    case Tanks::EGameState::Playing:
        break;
    case Tanks::EGameState::Quitting:
        break;
    default:
        break;
    }
}

void Tanks::TankGame::DrawMainMenu()
{
    ImGui::Begin( "Main Menu" );

    static char namebuf [ 64 ] = "Player 1\0";
    static char serverAddressBuf [ 64 ] = "127.0.0.1\0";
    static unsigned short serverPortBuf = 50001;
    ImGui::InputText( "Player Name", namebuf, IM_ARRAYSIZE( namebuf ) );
    ImGui::InputText( "Address", serverAddressBuf, IM_ARRAYSIZE( serverAddressBuf ) );
    ImGui::InputInt( "Port", ( int* ) &serverPortBuf );

    if ( ImGui::Button( "Connect", ImVec2( ImGui::GetWindowWidth(), 0.f ) ) )
    {
        // Attempt to connect to the server
        NetMan = ClientNetworkManager::StaticInit( serverAddressBuf, serverPortBuf, namebuf );
        GameState = EGameState::Playing;
    }

    ImGui::Separator();

    if ( ImGui::Button( "Quit", ImVec2( ImGui::GetWindowWidth(), 0.f ) ) )
    {
        Quit();
    }
    ImGui::End();
}