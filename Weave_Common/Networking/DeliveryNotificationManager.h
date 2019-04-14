#pragma once

#include <deque>

#include "MemoryBitStream.h"

typedef uint16_t PacketSequenceNumber;

/// <summary>
/// Keeps track of packet ordering and acks between server and client.
/// I based this off of the Chapter 8 robocat example from the following 
/// book: https://github.com/BenjaFriend/MultiplayerBook
/// </summary>
/// <author>Ben Hoffman</author>
class DeliveryNotificationManager
{
public:

    void WriteSequenceNumber( OutputMemoryBitStream& aPacket );

    bool ProcessSequenceNumber( InputMemoryBitStream & aPacket );



private:


    /** Deque of what packets are currently in flight */
    std::deque< PacketSequenceNumber > InFlightPackets;

    /** The next outgoing packet to be sent */
    PacketSequenceNumber NextOutgoingSequenceNum = 0;

    /** The next expected packet to be received */
    PacketSequenceNumber NextExpectedSequenceNum = 0;

    /** Stats */
    UINT32 NumPacketsDelivered = 0;
    UINT32 NumPacketsDropped = 0;
    UINT32 NumPacketsDispatched = 0;

};