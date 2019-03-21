#pragma once

#include "Networking/NetworkManager.h"

namespace Tanks
{
    class ClientNetworkManager : public NetworkManager
    {
    public:
        ClientNetworkManager();

        virtual ~ClientNetworkManager();


    protected:

        virtual void ProcessPacket( InputMemoryBitStream& inInputStream, const boost::asio::ip::udp::endpoint & inFromAddress ) override;

    private:



    };
}