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


class NetworkManager
{
public:
    NetworkManager();

    virtual ~NetworkManager();

    /** Remove the copy ctor and operator */
    NetworkManager( const NetworkManager & ) = delete;
    NetworkManager & operator=( const NetworkManager & ) = delete;

    bool Init( UINT16 aPort );

protected:

    /** Check for if we are done or not */
    std::atomic<bool> IsDone;

    /// <summary>
    /// Process a packet from the queue of packets
    /// </summary>
    virtual void ProcessPacket( InputMemoryBitStream& inInputStream, const boost::asio::ip::udp::endpoint & inFromAddress ) = 0;


    class ReceivedPacket
    {
    public:
        ReceivedPacket( float inRecievedtime, InputMemoryBitStream& inStream, boost::asio::ip::udp::endpoint & aInEndpoint );

        float GetReceivedTime() const { return Recievedtime; }
        const boost::asio::ip::udp::endpoint& GetEndpoint() const { return InEndpoint; }
        InputMemoryBitStream& GetPacketBuffer() { return PacketBuffer; }

    private:
        float Recievedtime;
        InputMemoryBitStream PacketBuffer;
        boost::asio::ip::udp::endpoint InEndpoint;
    };

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
    boost::asio::io_service io_service;

    /** the endpoint of the remote client contacting the server */
    boost::asio::ip::udp::endpoint remote_endpoint;

    /** Char buffer for storing messages */
    char recv_buf [ DEF_BUF_SIZE ];

    /** The queue of packets to process */
    std::queue< ReceivedPacket > PacketQueue;

};
