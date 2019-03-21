#pragma once

// Define that we are the server to help with dependency management
#define WEAVE_SERVER

#include <iostream>
#include <thread>
#include <memory>

#include "ServerNetworkManager.h"

#include "Room.h"
#include "MemoryBitStream.h"

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

    friend std::ostream& operator<<( std::ostream& os, const SERVER_INIT_DESC& data )
    {
        os << "\nServer Config";
        os << "\n\tListen Port: \t" << data.ListenPort;
        os << "\n\tResponse Port: \t" << data.ResponsePort;
        os << "\n\tMax Rooms: \t" << data.MaxRooms << "\n";
        return os;
    }
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
    * @brief    Shutdown the server
    */
    void Shutdown();

private:

    ServerNetworkManager* NetworkMan = nullptr;

    /** The max number of rooms that this server can have */
    unsigned int MaxRooms = 4;

    /** The default port to listen to data from the clients on  */
    unsigned short ListenPort = DEF_LISTEN_PORT;

    /** The default port to send data back to the clients on  */
    unsigned short ResponsePort = DEF_RESPONSE_PORT;

    /** Pointer to the room objects */
    Room* Rooms = nullptr;
};