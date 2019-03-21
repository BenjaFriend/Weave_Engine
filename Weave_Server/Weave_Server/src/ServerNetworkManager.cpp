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

void ServerNetworkManager::ProcessPacket()
{
    LOG_TRACE( "Server process packet!" );
}

void ServerNetworkManager::UpdateAllClients()
{
}
