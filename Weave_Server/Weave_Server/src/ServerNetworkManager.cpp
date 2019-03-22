#include "stdafx.h"

#include "ServerNetworkManager.h"

ServerNetworkManager::ServerNetworkManager()
    : NetworkManager()
{

}

ServerNetworkManager::~ServerNetworkManager()
{
    LOG_TRACE( "--- ServerNetworkManage DTOR ---" );
}

void ServerNetworkManager::ProcessPacket( InputMemoryBitStream& inInputStream, const boost::asio::ip::udp::endpoint & inFromAddress )
{
    UINT32 packetType;
    inInputStream.Read( packetType );

    LOG_TRACE( "From input stream: {} ", packetType );
}

void ServerNetworkManager::UpdateAllClients()
{
}
