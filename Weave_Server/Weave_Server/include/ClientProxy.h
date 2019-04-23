#pragma once

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include "Entity/Entity.h"
#include "Networking/DeliveryNotificationManager.h"

class ClientProxy
{

public:

    ClientProxy( const boost::asio::ip::udp::endpoint & inFromAddress, const std::string & aName, const UINT32 aID );

    ~ClientProxy();

    FORCE_INLINE const boost::asio::ip::udp::endpoint & GetEndpoint() const { return Endpoint; }

    FORCE_INLINE const std::string & GetName() const { return Name; }   

    FORCE_INLINE const UINT32 GetPlayerID() const { return PlayerId; }

    FORCE_INLINE const float GetLastPacketFromClientTime() const { return LastPacketFromClientTime; }

    DeliveryNotificationManager & GetDeliveryNotificationManager() { return NotifManager; }

    /// <summary>
    /// Update the time that this client knows it was last received from.
    /// </summary>
    void UpdateLastPacketTime();

    void SetClientEntity( Entity* aEnt ) { ClientEntity = aEnt; }

    Entity* GetClientEntity() { return ClientEntity; }

private:

    /** The remote endpoint of this client */
    boost::asio::ip::udp::endpoint Endpoint;

    /** The notification manager for this client */
    DeliveryNotificationManager NotifManager;

    /** Player's string name that they gave us */
    std::string Name;

    /** Player's unique ID */
    UINT32 PlayerId;

    /** pointer to this client's entity */
    Entity* ClientEntity = nullptr;

    /** The time that a packet was last received from this client */
    float LastPacketFromClientTime = 0.0f;

};

// Use smart pointers for the client proxy to have safer exits
typedef std::shared_ptr< ClientProxy >	ClientProxyPtr;