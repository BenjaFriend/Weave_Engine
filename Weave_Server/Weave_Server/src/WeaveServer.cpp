#include "WeaveServer.h"

namespace bai = boost::asio::ip;

WeaveServer::WeaveServer( SERVER_INIT_DESC aDesc )
    :
    MaxRooms( aDesc.MaxRooms ),
    ListenPort( aDesc.ListenPort ),
    ResponsePort( aDesc.ResponsePort )
{
    std::shared_ptr< bai::udp::socket > wat(
        new bai::udp::socket( io_service, bai::udp::endpoint( bai::udp::v4(), ListenPort ) )
    );

    // Excuse me
    ListenSocket = std::move( wat );

    // Create the rooms
    Rooms = new Room [ MaxRooms ];

    // Start the running thread
    runningThread = std::thread( &WeaveServer::Run, this );
}

WeaveServer::~WeaveServer()
{
    io_service.stop();

    if ( runningThread.joinable() )
    {
        runningThread.join();
    }

    // Clean up rooms
    if ( Rooms != nullptr )
    {
        delete [] Rooms;
        Rooms = nullptr;
    }

    std::cout << "Successful server dtor!" << std::endl;
}

size_t WeaveServer::Run()
{
    std::cout << "Server:: Start running!" << std::endl;

    StartRecieve();

    while ( !IsDone || !io_service.stopped() )
    {
        // Run said server
        io_service.run();

        std::cout << "Server iteration next!" << std::endl;

        // Start another async request
        StartRecieve();
    }

    std::cout << "Server:: Stop running!" << std::endl;

    return 0;
}

void WeaveServer::Shutdown()
{
    io_service.stop();
    IsDone = true;
}

void WeaveServer::StartRecieve()
{
    std::cout << "StartRecieve" << std::endl;

    ListenSocket->async_receive_from(
        boost::asio::buffer( recv_buf, DEF_BUF_SIZE ),
        remote_endpoint,
        boost::bind( &WeaveServer::HandleRemoteRecieved, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred )
    );
}

void WeaveServer::HandleRemoteRecieved( const std::error_code & error, std::size_t msgSize )
{
    if ( !error )
    {
        // access to remote_endpoint
        std::cout << "Message received! " << msgSize << std::endl;
        std::cout << recv_buf << std::endl;
    }
    else
    {
        std::cerr << "Message error! " << error << std::endl;
    }
}