#include "WeaveServer.h"

using boost::asio::ip::udp;

WeaveServer::WeaveServer( SERVER_INIT_DESC aDesc )
: ListenSocket( io_service, udp::endpoint( udp::v4(), aDesc.ListenPort ) )
{
    MaxRooms = aDesc.MaxRooms;
    ListenPort = aDesc.ListenPort;
    ResponsePort = aDesc.ResponsePort;

    Rooms = new Room[ MaxRooms ];

    io_service.run();

    // Create the running thread
    runningThread = std::thread( &WeaveServer::Run, this );
}

WeaveServer::~WeaveServer()
{    
    // Stop IO services (flags for running thread to stop)
    io_service.stop();

    // Wait for server thread to finish
    if( runningThread.joinable() )
    {
        runningThread.join();
    }

    // Clean up rooms
    if( Rooms != nullptr )
    {
        delete[] Rooms;
        Rooms = nullptr;
    }
    
    std::cout << "Successful server dtor!" << std::endl;
}

size_t WeaveServer::Run()
{
    std::cout << "Server:: Start running!" << std::endl;
    // Initalize the socket
    while( !io_service.stopped() )
    {
        // TODO: Invesigate a better exception handling system
        try 
        {
            io_service.run();
        }
        catch ( const std::exception & e ) 
        {
            std::cerr << "WaveServer: Network exception: " << e.what() << std::endl;
        }
        catch ( ... ) 
        {
            std::cerr <<  "Server: Network exception: unknown" << std::endl;
        }    
    }   // End service loop

    std::cout << "Server:: Stop running!" << std::endl;

    return 0;
}
