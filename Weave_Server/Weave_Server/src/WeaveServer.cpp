#include "WeaveServer.h"

using boost::asio::ip::tcp;

WeaveServer::WeaveServer( SERVER_INIT_DESC aDesc )
{
    MaxRooms = aDesc.MaxRooms;
    ListenPort = aDesc.ListenPort;
    ResponsePort = aDesc.ResponsePort;

    Rooms = new Room[ MaxRooms ];

    std::cout << "Server Ctor!" << std::endl;
}

WeaveServer::~WeaveServer()
{
    std::cout << "Server dtor!" << std::endl;
    if( Rooms != nullptr )
    {
        delete[] Rooms;
        Rooms = nullptr;
    }
}

size_t WeaveServer::Run()
{
    return 0;
}
