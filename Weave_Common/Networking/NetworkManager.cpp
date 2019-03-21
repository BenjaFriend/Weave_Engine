#include "stdafx.h"

#include "NetworkManager.h"

namespace bai = boost::asio::ip;

NetworkManager::NetworkManager()
{

}

NetworkManager::~NetworkManager()
{
    io_service.stop();
    IsDone = true;

    if ( runningThread.joinable() )
    {
        runningThread.join();
    }

    LOG_TRACE( " --- NetworkManager DTOR --- " );
}

bool NetworkManager::Init( UINT16 aPort )
{
    // Create a new UDP socket on the given port
    std::shared_ptr< bai::udp::socket > wat(
        new bai::udp::socket( io_service, bai::udp::endpoint( bai::udp::v4(), aPort ) )
    );

    ListenSocket = std::move( wat );

    LOG_TRACE( "Initalize Network manager at port {}", aPort );

    // Set non-blocking mode (if not using boost)

    // Start the running thread
    runningThread = std::thread( &NetworkManager::Run, this );

    return true;
}

void NetworkManager::ProcessPacket()
{

}

void NetworkManager::Run()
{
    StartRecieve();

    while ( !IsDone || !io_service.stopped() )
    {
        // Run said server
        io_service.run();

        LOG_TRACE( "Server iteration next!" );
    }
}

void NetworkManager::StartRecieve()
{
    // Start listening ASYNCRONOUSLY for incoming data
    // Set HandleRemoteRecieved to be called when there is data received
    // The endpoint information will be stored in 'remote_endpoint'
    ListenSocket->async_receive_from(
        boost::asio::buffer( recv_buf, DEF_BUF_SIZE ),
        remote_endpoint,
        boost::bind( &NetworkManager::HandleRemoteRecieved, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred )
    );
}

void NetworkManager::HandleRemoteRecieved( const std::error_code & error, std::size_t msgSize )
{
    // We have received some data, let's process it. 
    // The data is in the 'recv_buf' currently
    if ( !error )
    {
        // access to remote_endpoint
        LOG_TRACE( "Network Man: Message received! Size: {}", msgSize );
        LOG_WARN( "NetworkMan recv buf: {} ", recv_buf );
        // memcpy the buffer to a pack on the queue to be processed


        // Start another async request
        StartRecieve();
    }
    else
    {
        LOG_ERROR( "Network Man: Message error! {} ", error.message() );
    }
}