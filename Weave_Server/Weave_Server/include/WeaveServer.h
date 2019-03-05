#pragma once

#include <iostream>
#include <atomic>

#include <boost/lambda/lambda.hpp>
#include <boost/asio.hpp>
#include <boost/regex.hpp>

#include "Room.h"

#define DEF_LISTEN_PORT     50000
#define DEF_RESPONSE_PORT   50001

struct SERVER_INIT_DESC
{
    /** The default port to listen to data from the clients on  */
    unsigned short ListenPort   = DEF_LISTEN_PORT;

    /** The default port to send data back to the clients on  */
    unsigned short ResponsePort = DEF_RESPONSE_PORT;

    /** The max number of rooms that are allowed on this server */
    unsigned short MaxRooms     = 4;
};

/** 
 * @brief   The core server architecture with send and recieve sockets
 *          that will funnel the necessary data to the rooms.
 * 
 * @author  Ben Hoffman
 */
class WeaveServer
{
public:

    WeaveServer( SERVER_INIT_DESC aDesc );
    
    ~WeaveServer();

    /**
    * @brief    Start the execution of the server, handle initalization
    *           of sockets, rooms, etc.
    *
    * @return   Result of the server running
    */
    size_t Run();

private:

    /** Any necessary updates to the rooms */
    void Tick();

    /** The max number of rooms that this server can have */
    unsigned int MaxRooms = 4;

    /** The default port to listen to data from the clients on  */
    unsigned short ListenPort = DEF_LISTEN_PORT;

    /** The default port to send data back to the clients on  */
    unsigned short ResponsePort = DEF_RESPONSE_PORT;

    // Listen socket

    // Send socket

    /** Atomic flag to check if we are done */
    std::atomic<bool> isDone;
    
    /** Pointer to the room objects */
    Room* Rooms = nullptr;

};
