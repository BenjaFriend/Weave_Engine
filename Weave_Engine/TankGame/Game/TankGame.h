#pragma once

#include <Core/Game.h>
#include "../Networking/ClientNetworkManager.h"
#include "PlayerMoves.h"
#include <boost/asio.hpp>
#include <memory>

namespace Tanks
{

    enum class EGameState : UINT8
    {
        MainMenu,
        Playing,
        Quitting
    };

    class TankGame : public Game
    {

    public:

        TankGame( HINSTANCE hInstance );
        ~TankGame();

        /// <summary>
        /// Initalize core systems here
        /// </summary>
        virtual void Init() override;

        /// <summary>
        /// Main update tick of the game. Manage game state logic here
        /// </summary>
        /// <param name="deltaTime">Delta time</param>
        /// <param name="totalTime">total runtime of the game</param>
        virtual void Update( float deltaTime, float totalTime ) override;

        /// <summary>
        /// Quit the game
        /// </summary>
        virtual void Quit() override;

    protected:

        /// <summary>
        /// Draw the appropriate UI based on the current game state
        /// </summary>
        virtual void DrawUI() override;

        /// <summary>
        /// Draws the menu options
        /// </summary>
        void DrawMainMenu();
    
        /// <summary>
        /// Draw the UI that should appear during gameplay
        /// </summary>
        void DrawGameUI();

		///<summary>
		/// Callback that occurs when this client is disconnected
		///</summary>
		void Disconnect();

    private:

        /** Pointer to the io_service for the network manager
        * This has to be here because otherwise there is an exception on exit
        * in release mode when the netMan io_service goes out of context
        */
        std::shared_ptr < boost::asio::io_service > io_service;

        /** Network manager for this tank game */
        ClientNetworkManager* NetMan = nullptr;

        /** The current game state of this client */
        EGameState GameState = EGameState::MainMenu;  
    };

}   // namespace Tanks