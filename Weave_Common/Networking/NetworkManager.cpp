#include "stdafx.h"

#include "NetworkManager.h"

namespace bai = boost::asio::ip;

NetworkManager::NetworkManager()
{

}

NetworkManager::~NetworkManager()
{
    io_service.stop();

    //LOG_TRACE( "NetworkManager DTOR" );
}

bool NetworkManager::Init( UINT16 aPort )
{
    // Create a new UDP socket on the given port
    std::shared_ptr< bai::udp::socket > wat(
        new bai::udp::socket( io_service, bai::udp::endpoint( bai::udp::v4(), aPort ) )
    );

    ListenSocket = std::move( wat );

    //LOG_TRACE( "Initalize Network manager at port {}", aPort );

    // Set non-blocking mode (if not using boost)

    return true;
}

void NetworkManager::StartRecieve()
{
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
    if ( !error )
    {
        // access to remote_endpoint
        std::cout << "Network Man: Message received! " << msgSize << std::endl;
        std::cout << recv_buf << std::endl;

        // Start another async request
        StartRecieve();
    }
    else
    {
        std::cerr << "Network Man: Message error! " << error << std::endl;
    }
}