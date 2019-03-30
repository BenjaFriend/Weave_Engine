#include "stdafx.h"

#include "NetworkManager.h"

namespace bai = boost::asio::ip;

std::shared_ptr< boost::asio::ip::udp::socket > UDPSocketFactory( 
    boost::asio::io_service & service, bai::udp::endpoint& aEnpoint )
{
    return std::make_shared< boost::asio::ip::udp::socket >( service, aEnpoint );
}

NetworkManager::NetworkManager( std::shared_ptr< boost::asio::io_service > aServce )
{
    // Create an IO service
    //io_service = std::make_shared< boost::asio::io_service >();
    io_service = aServce;
}

NetworkManager::~NetworkManager()
{
    io_service->stop();
    IsDone = true;

    if ( runningThread.joinable() )
    {
        runningThread.join();
    }

    LOG_TRACE( " --- NetworkManager DTOR --- " );
}

bool NetworkManager::Init( UINT16 aPort )
{
    ListenSocket = UDPSocketFactory( *io_service, bai::udp::endpoint( bai::udp::v4(), aPort ) );

    LOG_TRACE( "Initalize Network manager at port {}", aPort );

    // Set non-blocking mode (if not using boost)

    // Start the running thread
    runningThread = std::thread( &NetworkManager::Run, this );

    return true;
}

void NetworkManager::ProcessIncomingPackets()
{
    // If we were not using Boost asio, then we would do a recvfrom socket call here

    ProcessQueuedPackets();


}

void NetworkManager::SendPacket( const OutputMemoryBitStream & inOutputStream, const bai::udp::endpoint & inFromAddress )
{
    // Send the output stream to the given endpoint
    // returns the number of bytes send
    ListenSocket->send_to(
        boost::asio::buffer( inOutputStream.GetBufferPtr(), inOutputStream.GetBitLength() ),
        inFromAddress
    );
    // #TODO Keep track of the number of bytes sent this frame
}

void NetworkManager::Run()
{
    StartRecieve();

    while ( !IsDone || !io_service->stopped() )
    {
        // Run said server
        try
        {
            io_service->run();
        }
        catch ( const std::exception& e )
        {
            LOG_ERROR( "Server: Network exception: {}", e.what() );
        }
        catch ( ... )
        {
            LOG_ERROR( "Server: Network exception: UNKNOWN" );
        }
    }
}

void NetworkManager::ProcessQueuedPackets()
{
    while ( !PacketQueue.empty() )
    {
        ReceivedPacket& nextPacket = PacketQueue.front();
        // #TODO Only process this packet if it was received later than the last one
        ProcessPacket( nextPacket.GetPacketBuffer(), nextPacket.GetEndpoint() );
        PacketQueue.pop();
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
        // Put the data into an input stream so that we can manage it
        int packetSize = sizeof( recv_buf );
        InputMemoryBitStream inputStream( recv_buf, packetSize * 8 );

        // Put this packet memory into the buffer
        ReceivedPacket packet( 0.0f, inputStream, remote_endpoint );
        PacketQueue.emplace( packet );

        // Start another async request
        StartRecieve();
    }
    else
    {
        LOG_ERROR( "Network Man: Message error! {} ", error.message() );
    }
}

NetworkManager::ReceivedPacket::ReceivedPacket( float inRecievedtime, InputMemoryBitStream & inStream, boost::asio::ip::udp::endpoint & aInEndpoint ) :
    Recievedtime( inRecievedtime ),
    PacketBuffer( inStream ),
    InEndpoint( aInEndpoint )
{
}