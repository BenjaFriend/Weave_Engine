#pragma once

#include  <boost/asio.hpp>
#include <string>
#include <memory>

class  ClientProxy
{

public:

    ClientProxy( const boost::asio::ip::udp::endpoint & inFromAddress, const std::string & aName, const UINT32 aID );

    ~ClientProxy();

    FORCE_INLINE const boost::asio::ip::udp::endpoint & GetEndpoint() const { return Endpoint; }

    FORCE_INLINE const std::string & GetName() const { return Name; }

    FORCE_INLINE const UINT32 GetPlayerID() const { return PlayerId; }

private:

    /** The remote endpoint of this client */
    boost::asio::ip::udp::endpoint Endpoint;

    /** Player's string name that they gave us */
    std::string Name;

    /** Player's unique ID */
    UINT32 PlayerId;
};

// Use smart pointers for the client proxy to have safer exits
typedef std::shared_ptr< ClientProxy >	ClientProxyPtr;