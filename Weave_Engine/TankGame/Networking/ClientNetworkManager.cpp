#include "pch.h"
#include "ClientNetworkManager.h"

using namespace Tanks;

ClientNetworkManager::ClientNetworkManager()
    : NetworkManager()
{

}

ClientNetworkManager::~ClientNetworkManager()
{

}

void ClientNetworkManager::ProcessPacket( InputMemoryBitStream& inInputStream, const boost::asio::ip::udp::endpoint & inFromAddress )
{

}