#include "stdafx.h" 

#include "WeaveServer.h"

WeaveServer::WeaveServer( SERVER_INIT_DESC aDesc )
    :
    MaxRooms( aDesc.MaxRooms ),
    ListenPort( aDesc.ListenPort ),
    ResponsePort( aDesc.ResponsePort ),
    TimeBetweenStateUpdates( aDesc.StateUpdateTickRate )
{
    NetworkMan = new ServerNetworkManager();
    NetworkMan->Init( ListenPort );

    // Add user command options
    UserOptions [ "QUIT" ] = EUserCoptions::QUIT;
    UserOptions [ "HELP" ] = EUserCoptions::HELP;
    ShouldQuit = false;
}

WeaveServer::~WeaveServer()
{
    SAFE_DELETE( NetworkMan );
    
    // Make sure that user input thread is cleaned up
    ShouldQuit = true;
    if ( userInputThread.joinable() )
    {
        userInputThread.join();
    }

    // Clean up rooms
    if ( Rooms != nullptr )
    {
        delete [] Rooms;
        Rooms = nullptr;
    }

    std::cout << "Successful server dtor!" << std::endl;
}

void WeaveServer::Run()
{
    // Spawn a thread for user input
    userInputThread = std::thread( &WeaveServer::ProcessConsoleInput, this );

    while ( 1 )
    {
        if ( ShouldQuit ) break;

        Timing::sInstance.Update();
        DeltaTime = Timing::sInstance.GetDeltaTime();
        TotalTime = Timing::sInstance.GetTimef();

        NetworkMan->ProcessIncomingPackets();

        // If we have hit our tick rate, then update all clients
        if ( TotalTime > TimeOfLastStateUpdate + TimeBetweenStateUpdates )
        {
            NetworkMan->UpdateAllClients();
            TimeOfLastStateUpdate = TotalTime;
        }

        // Update the rooms and scenes here
    }
    
}

void WeaveServer::Shutdown()
{
    LOG_TRACE( "Shut Down server!" );
    ShouldQuit = true;
}

void WeaveServer::ProcessConsoleInput()
{
    std::string input = "";
    bool isDone = false;
    // Input loop for stopping the server
    while ( !isDone )
    {
        std::cout << "Enter a command [ QUIT ]: ";
        std::cin >> input;
        std::string cmd;

        // Put the input to all upper case into the "cmd" variable
        std::transform( input.begin(), input.end(), std::back_inserter( cmd ), ::toupper );

        if ( UserOptions.find( cmd ) == UserOptions.end() )
        {
            std::cout << "\t" << cmd << " is not a valid command!" << std::endl;
            continue;
        }

        switch ( UserOptions [ cmd ] )
        {
        case EUserCoptions::QUIT:
            std::cout << "Quitting..." << std::endl;
            Shutdown();
            isDone = true;
            break;

        case EUserCoptions::HELP:
            std::cout << "Command options: " << std::endl;
            for ( auto const & possibleCmd : UserOptions )
            {
                std::cout << "\t" << possibleCmd.first << std::endl;
            }
            std::cout << "\n\n" << std::endl;
            break;
        default:
            std::cout << "\t" << cmd << " is not a valid command!" << std::endl;
            break;
        }

    }
}