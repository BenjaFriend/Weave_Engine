#pragma once


#include "MemoryBitStream.h"

#include <queue>
#include <thread>
#include <memory>
#include <iostream>

#include <boost/lambda/lambda.hpp>
#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <boost/bind.hpp>
#include <boost/asio/buffer.hpp>

#define DEF_BUF_SIZE        512

/// <summary>
/// Base network manager provides the ability to easily send and receive
/// data. This implementation goes off of the book 'Multiplayer Game Programming: Architecting Networked Games'
/// Code examples that I followed can be found here: 
/// https://github.com/MultiplayerBook/MultiplayerBook/tree/master/Chapter%208/RoboCatAction
/// </summary>
class NetworkManager
{

public:

    // Packet types
    static const UINT32 HelloPacket         = 'HELO';   // Hello packet from the client
    static const UINT32 WelcomePacket       = 'WELC';   // Welcome packet to initialize the client
    static const UINT32 StatePacket         = 'STAT';   // State update of the scene
    static const UINT32	InputPacket         = 'INPT';   // The client's input state
    static const UINT32	FeedMessagePacket   = 'FMSG';   // A message ment to give a feed update to all the client

    NetworkManager( std::shared_ptr< boost::asio::io_service > aServce );

    virtual ~NetworkManager();

    /** Remove the copy ctor and operator */
    NetworkManager( const NetworkManager & ) = delete;
    NetworkManager & operator=( const NetworkManager & ) = delete;

    bool Init( UINT16 aPort );

    /// <summary>
    /// Check and see if there are any packets incoming to this socket
    /// </summary>
    void ProcessIncomingPackets();

    /// <summary>
    /// Send a pack to the  given destination
    /// </summary>
    /// <param name="inOutputStream">Data to send within this packet</param>
    /// <param name="inFromAddress">The endpoint to send this data to</param>
    void SendPacket( const OutputMemoryBitStream& inOutputStream, const boost::asio::ip::udp::endpoint & inFromAddress );

protected:

    /** Check for if we are done or not */
    std::atomic<bool> IsDone;

    /// <summary>
    /// Process a packet from the queue of packets
    /// </summary>
    virtual void ProcessPacket( InputMemoryBitStream& inInputStream, const boost::asio::ip::udp::endpoint & inFromAddress ) = 0;

    /// <summary>
    /// A packet is data that has been received from a remote endpoint. Process the information
    /// inside this packet by using it's bit stream. 
    /// </summary>
    class ReceivedPacket
    {
    public:
        ReceivedPacket( float inRecievedtime, InputMemoryBitStream& inStream, boost::asio::ip::udp::endpoint & aInEndpoint );

        float GetReceivedTime() const { return Recievedtime; }
        const boost::asio::ip::udp::endpoint& GetEndpoint() const { return InEndpoint; }
        InputMemoryBitStream& GetPacketBuffer() { return PacketBuffer; }

    private:

        /** Time that this packet was received */
        float Recievedtime;

        /** Data buffer of this packet */
        InputMemoryBitStream PacketBuffer;

        /** The endpoint that this packet came from */
        boost::asio::ip::udp::endpoint InEndpoint;
    };

    static std::shared_ptr< boost::asio::ip::udp::socket > UDPSocketFactory(
        boost::asio::io_service & service, const boost::asio::ip::udp::endpoint& aEnpoint )
    {
        return std::make_shared< boost::asio::ip::udp::socket >( service, aEnpoint );
    }

private:

    /// <summary>
    /// Start the execution of listening 
    /// </summary>
    void Run();

    /// <summary>
    /// Process the packets that are currently in the queue
    /// </summary>
    void ProcessQueuedPackets();

    /// <summary>
    /// Hand a remove connection coming in via UDP from an endpoint
    /// </summary>
    /// <param name="error">Error message if failed</param>
    /// <param name="msgSize">the size of the message that was received</param>
    void HandleRemoteRecieved( const std::error_code & error, std::size_t msgSize );

    /// <summary>
    /// Start an Async receive
    /// </summary>
    void StartRecieve();

    /** The running thread of waiting for data */
    std::thread runningThread;

    /** Socket that is used for listening */
    std::shared_ptr< boost::asio::ip::udp::socket > ListenSocket;

    /** Io service for running the sockets */
    std::shared_ptr < boost::asio::io_service > io_service;

    /** the endpoint of the remote client contacting the server */
    boost::asio::ip::udp::endpoint remote_endpoint;

    /** Char buffer for storing messages */
    char recv_buf [ DEF_BUF_SIZE ];

    /** The queue of packets to process */
    std::queue< ReceivedPacket > PacketQueue;   // #TODO Make this is a lockless queue

};
