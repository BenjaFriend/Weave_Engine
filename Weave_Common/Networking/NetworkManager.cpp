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

    while ( !IsDone || !io_service.stopped() )
    {
        // Run said server
        io_service.run();

        LOG_TRACE( "Server iteration next!" );
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
    InEndpoint ( aInEndpoint )
{
}