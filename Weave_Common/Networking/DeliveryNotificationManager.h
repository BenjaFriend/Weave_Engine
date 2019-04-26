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

    DeliveryNotificationManager( bool shouldSendAcks, bool shuoldProcessAcks );

    ~DeliveryNotificationManager();

    /// <summary>
    /// Process packets that have not been timed out yet
    /// </summary>
    void ProcessTimedOutPackets();
    
    /// <summary>
    /// Write the state of the ACK for this packet
    /// </summary>
    /// <param name="inOutputStream">The packet to </param>
    void WriteState( OutputMemoryBitStream & inOutputStream );

    /// <summary>
    /// Read the ACK from the given packet and check if it is valid
    /// </summary>
    /// <param name="inInputStream">The packet to check the ACK on</param>
    /// <returns>True if this state packet is valid</returns>
    bool ReadAndProcessState( InputMemoryBitStream & inInputStream );

    UINT32 GetNumPacketsDelivered() const { return NumPacketsDelivered; }
    UINT32 GetNumPacketsDropped() const { return NumPacketsDropped; }
    UINT32 GetNumPacketsDispatched() const { return NumPacketsDispatched; }

    const std::deque< PacketSequenceNumber > & GetInFlightPackets() const { return InFlightPackets; }

private:

    void WriteSequenceNumber( OutputMemoryBitStream& aPacket );

    bool ProcessSequenceNumber( InputMemoryBitStream & aPacket );

    /** The next outgoing packet to be sent */
    PacketSequenceNumber NextOutgoingSequenceNum = 0;

    /** The next expected packet to be received */
    PacketSequenceNumber NextExpectedSequenceNum = 0;

    /** Deque of what packets are currently in flight */
    std::deque< PacketSequenceNumber > InFlightPackets;

    /** Flags for what this manager should be doing with it's Acks */
    bool ShouldSendAcks = false;
    bool ShouldProcessAcks = false;

    /** Stats */
    UINT32 NumPacketsDelivered = 0;
    UINT32 NumPacketsDropped = 0;
    UINT32 NumPacketsDispatched = 0;

};