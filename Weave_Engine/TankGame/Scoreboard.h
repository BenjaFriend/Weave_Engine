#pragma once
#include "MemoryBitStream.h"
#include <basetsd.h>
#include <imgui/imgui.h>
#include <vector>
#include "Entity/Entity.h"


class Scoreboard
{
public:

	Scoreboard();
	~Scoreboard();

	void Read(UINT8 playerCount, InputMemoryBitStream& inInputStream);

	void DrawUI(Entity* entityArray);
};

