#include "stdafx.h"
#include "DeliveryNotificationManager.h"

void DeliveryNotificationManager::WriteSequenceNumber( OutputMemoryBitStream & aPacket )
{

    PacketSequenceNumber sequenceNum = NextOutgoingSequenceNum++;
    aPacket.Write( sequenceNum );

    ++NumPacketsDispatched;

    InFlightPackets.emplace_back( sequenceNum );
}

bool DeliveryNotificationManager::ProcessSequenceNumber( InputMemoryBitStream & aPacket )
{


    return false;
}
