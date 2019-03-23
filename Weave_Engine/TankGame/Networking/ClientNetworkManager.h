#pragma once

#include "Networking/NetworkManager.h"

namespace Tanks
{
    /// <summary>
    /// Network manager for a client that can be used to process incoming and outgoing 
    /// packet data and control the online state of this player. 
    /// </summary>
    class ClientNetworkManager : public NetworkManager
    {
        /// <summary>
        /// The network state of this client that can be used to determine what
        /// outgoing information needs to be processed
        /// </summary>
        enum class EClientState : UINT8
        {
            Uninitalized,   // Client is uninitialized and at the main menu
            SayingHello,    // Client is attempting to connect to the server
            Welcomed        // Client has been welcomed into the game by the server
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
        void SendOutgoingPackets( float totalTime );

        /// <summary>
        /// Get the current state of the client
        /// </summary>
        /// <returns>Enum of the networked client state</returns>
        FORCE_INLINE const EClientState GetClientState() const { return ClientState; }
        FORCE_INLINE const UINT32 GetPlayerID() const { return PlayerID; }
        FORCE_INLINE const std::string & GetName() const { return Name; }
        FORCE_INLINE const boost::asio::ip::udp::endpoint  & GetServerEndpoint() { return ServerEndpoint; }

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

        /// <summary>
        /// Send a hello packet to the server
        /// </summary>
        void SendHelloPacket();

        /** The endpoint of the game server */
        boost::asio::ip::udp::endpoint ServerEndpoint;

        /** Connection state of this client */
        EClientState ClientState = EClientState::Uninitalized;

        /** The name of the player */
        std::string Name;

        /** The UNique ID of this player that is assigned to us from the server */
        UINT32 PlayerID;


        /** The last known time that we have sent a hello packet */
        float TimeOfLastHello;
        /** The amount of time between sending hello packets */
        const float TimeBetweenHellos = 1.f;


        /** Time between input updates */
        float TimeOfLastInputUpdate;
        /** The amount of time between sending hello packets */
        const float TimeBetweenInputUpdate = 1.f;

    };

}   // namespace Tanks