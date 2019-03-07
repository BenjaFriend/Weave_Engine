#pragma once

#include <iostream>
#include <thread>
#include <memory>

#include <boost/lambda/lambda.hpp>
#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <boost/bind.hpp>
#include <boost/asio/buffer.hpp>

#include "Room.h"

#define DEF_LISTEN_PORT     50000
#define DEF_RESPONSE_PORT   50001
#define DEF_BUF_SIZE        512

struct SERVER_INIT_DESC
{
    /** The default port to listen to data from the clients on  */
    unsigned short ListenPort = DEF_LISTEN_PORT;

    /** The default port to send data back to the clients on  */
    unsigned short ResponsePort = DEF_RESPONSE_PORT;

    /** The max number of rooms that are allowed on this server */
    unsigned short MaxRooms = 4;
};

/**
 * @brief   The core server architecture with send and receive sockets
 *          that will funnel the necessary data to the rooms.
 *
 * @author  Ben Hoffman
 */
class WeaveServer
{
public:

    WeaveServer( SERVER_INIT_DESC aDesc );

    /** Remove the copy ctor and operator */
    WeaveServer( const WeaveServer & ) = delete;
    WeaveServer & operator=( const WeaveServer & ) = delete;

    ~WeaveServer();

    /**
    * @brief    Start the execution of the server, handle initialization
    *           of sockets, rooms, etc.
    *
    * @return   Result of the server running
    */
    size_t Run();

    /**
    * @brief    Shutdown the server
    */
    void Shutdown();

private:

    /** Hand a remove connection coming in via UDP from an endpoint */
    void HandleRemoteRecieved( const std::error_code & error, std::size_t msgSize );

    /** Start an async receive */
    void StartRecieve();

    /** The server socket */
    std::shared_ptr< boost::asio::ip::udp::socket > ListenSocket;

    /** the endpoint of the remote client contacting the server */
    boost::asio::ip::udp::endpoint remote_endpoint;

    /** Io service for running the sockets */
    boost::asio::io_service io_service;

    /** Char buffer for storing messages */
    char recv_buf [ DEF_BUF_SIZE ];

    /** The running thread of waiting for data */
    std::thread runningThread;

    /** Check for if we are done or not */
    std::atomic<bool> IsDone;

    /** The max number of rooms that this server can have */
    unsigned int MaxRooms = 4;

    /** The default port to listen to data from the clients on  */
    unsigned short ListenPort = DEF_LISTEN_PORT;

    /** The default port to send data back to the clients on  */
    unsigned short ResponsePort = DEF_RESPONSE_PORT;

    /** Pointer to the room objects */
    Room* Rooms = nullptr;
};