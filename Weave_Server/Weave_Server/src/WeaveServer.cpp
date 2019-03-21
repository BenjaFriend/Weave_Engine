#include "stdafx.h" 

#include "WeaveServer.h"

WeaveServer::WeaveServer( SERVER_INIT_DESC aDesc )
    :
    MaxRooms( aDesc.MaxRooms ),
    ListenPort( aDesc.ListenPort ),
    ResponsePort( aDesc.ResponsePort )
{
    NetworkMan = new ServerNetworkManager();
    NetworkMan->Init( ListenPort );
}

WeaveServer::~WeaveServer()
{
    SAFE_DELETE( NetworkMan );

    // Clean up rooms
    if ( Rooms != nullptr )
    {
        delete [] Rooms;
        Rooms = nullptr;
    }

    std::cout << "Successful server dtor!" << std::endl;
}

void WeaveServer::Shutdown()
{
    LOG_TRACE( "Shut Down server!" );
}