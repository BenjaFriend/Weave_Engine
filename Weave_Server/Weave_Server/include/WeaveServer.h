#pragma once

#include <iostream>
#include <atomic>

#include <boost/lambda/lambda.hpp>
#include <boost/asio.hpp>
#include <boost/regex.hpp>

#include "Room.h"


class WeaveServer
{
public:
    WeaveServer();
    
    ~WeaveServer();

    /**
    * @brief    Start the execution of the server, handle initalization
    *           of sockets, rooms, etc.
    *
    * @return   Result of the server running
    */
    size_t Run();

private:

    /** The max number of rooms that this server can have */
    unsigned int MaxRooms = 4;



};
