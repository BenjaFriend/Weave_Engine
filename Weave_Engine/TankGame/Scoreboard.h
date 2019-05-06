#pragma once
#include "../Weave_Common/Networking/IScoreboard.h"

class Scoreboard : public IScoreboard
{
public:
	Scoreboard();
	~Scoreboard();


	void Read(InputMemoryBitStream& inInputStream, UINT8 playerCount);

	void Write(OutputMemoryBitStream& inOutputStream) const;

	void DrawUI() const;

private:

	std::unordered_map<std::string, int> clientScores;
};

