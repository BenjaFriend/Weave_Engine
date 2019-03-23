#pragma once

#include "Networking/NetworkManager.h"

namespace Tanks
{
    class ClientNetworkManager : public NetworkManager
    {
        enum class EClientState : UINT8
        {
            Uninitalized,
            SayingHello,
            Welcomed
        };

    public:
        static ClientNetworkManager* Instance;

        static ClientNetworkManager* StaticInit( const char * aServerAddr, const unsigned short aPort, const std::string& aName );

        /// <summary>
        /// Delete the static instance of the network manager
        /// </summary>
        static void ReleaseInstance();

        /// <summary>
        /// Send outgoing packets based on the current state of the client
        /// </summary>
        void SendOutgoingPackets();

        /// <summary>
        /// Get the current state of the client
        /// </summary>
        /// <returns>Enum of the networked client state</returns>
        FORCE_INLINE const EClientState GetClientState() const { return ClientState; }

    protected:

        ClientNetworkManager( const char * aServerAddr, const unsigned short aPort, const std::string& aName );

        virtual ~ClientNetworkManager();

        /// <summary>
        /// Process the given input stream of data
        /// </summary>
        /// <param name="inInputStream">Input stream of this packets data</param>
        /// <param name="inFromAddress">Address that this packet is from</param>
        virtual void ProcessPacket( InputMemoryBitStream& inInputStream, const boost::asio::ip::udp::endpoint & inFromAddress ) override;

    private:     

        /** The endpoint of the game server */
        boost::asio::ip::udp::endpoint ServerEndpoint;

        /** Connection state of this client */
        EClientState ClientState = EClientState::Uninitalized;

        /** The name of the player */
        std::string Name;


    };
}   // namespace Tanks