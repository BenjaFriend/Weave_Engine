#pragma once
#include "MemoryBitStream.h"
#include <basetsd.h>
#include "Entity/Entity.h"


/// <summary>
/// Base scoreboard class for a client and server.
/// To keep a consistent read/write packet format
/// </summary>
class IScoreboard
{
public:

	IScoreboard() {}
	virtual ~IScoreboard() {}

	/// <summary>
	/// Read in the scoreboard data from the packet
	/// Should only be called/used by client scoreboard
	/// </summary>
	/// <param name="playerCount">The amount of player scores to read </param>
	/// <param name="inInputStream">Input stream of this packets data</param>
	virtual void Read(UINT8 playerCount, InputMemoryBitStream& inInputStream) {}

	/// <summary>
	/// Write out the scoreboard data to the packet
	/// Should only be called/used by server scoreboard
	/// </summary>
	/// <param name="inOutputStream"></param>
	virtual void Write(OutputMemoryBitStream& inOutputStream) const {}
};

