#pragma once
#include "MemoryBitStream.h"
#include <basetsd.h>
#include "Entity/Entity.h"


class IScoreboard
{
public:

	IScoreboard() {}
	virtual ~IScoreboard() {}

	virtual void Read(UINT8 playerCount, InputMemoryBitStream& inInputStream) {}

	virtual void Write(OutputMemoryBitStream& inOutputStream) const {}
};

