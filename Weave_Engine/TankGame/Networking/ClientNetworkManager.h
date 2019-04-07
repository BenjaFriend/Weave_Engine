#pragma once

#include "Networking/NetworkManager.h"
#include "../Game/PlayerMoves.h"
#include <deque>

namespace Tanks
{

    /// <summary>
    /// Network manager for a client that can be used to process incoming and outgoing 
    /// packet data and control the online state of this player. 
    /// </summary>
    class ClientNetworkManager : public NetworkManager
    {

    public:
        /// <summary>
        /// The network state of this client that can be used to determine what
        /// outgoing information needs to be processed
        /// </summary>
        enum class EClientState : UINT8
        {
            Uninitalized,   // Client is uninitialized and at the main menu
            SayingHello,    // Client is attempting to connect to the server
            Welcomed,       // Client has been welcomed into the game by the server
            Leaving         // Client has been welcomed into the game by the server
        };

        static ClientNetworkManager * Instance;

        static ClientNetworkManager * StaticInit (
            std::shared_ptr< boost::asio::io_service > aService,
            const char * aServerAddr,
            const unsigned short aPort,
            const std::string & aName );

        /// <summary>
        /// Delete the static instance of the network manager
        /// </summary>
        static void ReleaseInstance ();

        /// <summary>
        /// Send outgoing packets based on the current state of the client
        /// </summary>
        void SendOutgoingPackets ( float totalTime );

        /// <summary>
        /// Get the current state of the client
        /// </summary>
        /// <returns>Enum of the networked client state</returns>
        FORCE_INLINE const EClientState GetClientState () const { return ClientState; }
        FORCE_INLINE const UINT32 GetPlayerID () const { return PlayerID; }
        FORCE_INLINE const std::string & GetName () const { return Name; }
        FORCE_INLINE const boost::asio::ip::udp::endpoint & GetServerEndpoint () { return ServerEndpoint; }
        FORCE_INLINE const UINT8 GetNumConnectedPlayers() const { return NumConnectedPlayers; }
        
    protected:

        ClientNetworkManager ( std::shared_ptr< boost::asio::io_service > aService,
                               const char * aServerAddr,
                               const unsigned short aPort,
                               const std::string & aName );

        virtual ~ClientNetworkManager ();

        /// <summary>
        /// Process the given input stream of data
        /// </summary>
        /// <param name="inInputStream">Input stream of this packets data</param>
        /// <param name="inFromAddress">Address that this packet is from</param>
        virtual void ProcessPacket ( InputMemoryBitStream & inInputStream, const boost::asio::ip::udp::endpoint & inFromAddress ) override;

    private:

        /// <summary>
        /// Send a hello packet to the server
        /// </summary>
        void SendHelloPacket ();

        /// <summary>
        /// Send the input state of this client to the server
        /// </summary>
        void SendInputPacket ();

        /// <summary>
        /// Send a 'heartbeat' packet to the server letting them know that we 
        /// are still connected!
        /// </summary>
        void SendHeartbeatPacket();

        /// <summary>
        /// Process the given  state packet from the server
        /// </summary>
        /// <param name="inInputStream">Input stream of data</param>
        void ProcessStatePacket ( InputMemoryBitStream & inInputStream );

        /** The endpoint of the game server */
        boost::asio::ip::udp::endpoint ServerEndpoint;

        /** Connection state of this client */
        EClientState ClientState = EClientState::Uninitalized;

        /** The name of the player */
        std::string Name;

        /** The UNique ID of this player that is assigned to us from the server */
        UINT32 PlayerID;

        /** The current number of connected players in this game */
        UINT8 NumConnectedPlayers = 0;

        /** The last known time that we have sent a hello packet */
        float TimeOfLastHello;
        /** The amount of time between sending hello packets */
        const float TimeBetweenHellos = 1.f;


        /** Time between input updates */
        float TimeOfLastInputUpdate;
        /** The amount of time between sending hello packets */
        const float TimeBetweenInputUpdate = 0.25f;

        /** Time between input updates */
        float TimeOfLastHeartbeat;
        /** The amount of time between sending hello packets */
        const float TimeBetweenHeartBeats = 0.5f;

        /** Keep track of the last time we received a state packet  */
        float TimeOfLastStatePacket = TimeUntilTimeout;
        /** The amount of time before this client times out from the server */
        const float TimeUntilTimeout = 10.0f;

    };

}   // namespace Tanks