#include "stdafx.h"
#include "DeliveryNotificationManager.h"

namespace
{
    const float DelayBeforeAckTimeout = 0.5f;
}

DeliveryNotificationManager::DeliveryNotificationManager( bool aShouldSendAcks, bool aShouldProcessAcks )
    : ShouldSendAcks( aShouldSendAcks ), ShouldProcessAcks( aShouldProcessAcks )
{

}

DeliveryNotificationManager::~DeliveryNotificationManager()
{
    LOG_TRACE( "Delivery Manager DTOR" );
}

void DeliveryNotificationManager::ProcessTimedOutPackets()
{
    float timeoutTime = Timing::sInstance.GetTimef() - DelayBeforeAckTimeout;

    //while ( !InFlightPackets.empty() )
    //{
    //
    //}

}

void DeliveryNotificationManager::WriteState( OutputMemoryBitStream & inOutputStream )
{
    WriteSequenceNumber( inOutputStream );
    if ( ShouldSendAcks )
    {
        // Write Ack Data
    }
    // Return InFlight packet info?
}

bool DeliveryNotificationManager::ReadAndProcessState( InputMemoryBitStream & inInputStream )
{
    bool iRet = ProcessSequenceNumber( inInputStream );

    if ( ShouldProcessAcks )
    {
        // #TODO: Process acks
    }

    return iRet;
}

void DeliveryNotificationManager::WriteSequenceNumber( OutputMemoryBitStream & aPacket )
{

    PacketSequenceNumber sequenceNum = NextOutgoingSequenceNum++;
    aPacket.Write( sequenceNum );

    ++NumPacketsDispatched;

    InFlightPackets.emplace_back( sequenceNum );
}

bool DeliveryNotificationManager::ProcessSequenceNumber( InputMemoryBitStream & aPacket )
{
    PacketSequenceNumber sequenceNum = {};
    aPacket.Read( sequenceNum );

    // Is this packet the expected ACK or newer? 
    if ( sequenceNum >= NextExpectedSequenceNum )
    {
        // This is what we expect, we should process this packet
        NextExpectedSequenceNum = sequenceNum + 1;
        return true;
    }
    else
    {
        // This packet is older than the current state, so drop it.
        return false;
    }

    return false;
}
