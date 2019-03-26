#include "stdafx.h"
#include  "ClientProxy.h"

ClientProxy::ClientProxy( const boost::asio::ip::udp::endpoint & inFromAddress, const std::string & aName, const UINT32 aID )
    : Endpoint( inFromAddress ), Name( aName ), PlayerId( aID )
{
}

ClientProxy::~ClientProxy()
{
}
