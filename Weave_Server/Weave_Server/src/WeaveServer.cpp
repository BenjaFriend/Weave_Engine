#include "WeaveServer.h"

using boost::asio::ip::udp;

WeaveServer::WeaveServer( SERVER_INIT_DESC aDesc )
: ListenSocket( io_service, udp::endpoint( udp::v4(), aDesc.ListenPort ) )
{
    MaxRooms = aDesc.MaxRooms;
    ListenPort = aDesc.ListenPort;
    ResponsePort = aDesc.ResponsePort;

    Rooms = new Room[ MaxRooms ];

    if ( io_service.stopped() )
    {
        std::cout << "Io service is stopped!" << std::endl;
    }

    // Create the running thread
    runningThread = std::thread( &WeaveServer::Run, this );
    runningThread.join();
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
    StartRecieve();
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
        std::cout << "Listening..." << std::endl;
    }   // End service loop

    std::cout << "Server:: Stop running!" << std::endl;

    return 0;
}

void WeaveServer::StartRecieve()
{
    ListenSocket.async_receive_from( 
        boost::asio::buffer( recv_buf, DEF_BUF_SIZE ), 
        remote_endpoint,
	    [this](std::error_code ec, std::size_t bytes_recvd ) { this->HandleRemoteRecieved( ec, bytes_recvd ); } );
}

void WeaveServer::HandleRemoteRecieved( const std::error_code & error, size_t msgSize )
{
    if( !error )
    {
        // access to remote_endpoint
        std::cout << "Message recieved! " << msgSize << std::endl;
    }
    else
    {
        std::cerr << "Message error!" << std::endl;
    }

    // Start another async request
    StartRecieve();
}
