#pragma once
#include "../Weave_Common/Networking/IScoreboard.h"


/// <summary>
/// Derived scoreboard class for clients
/// Reads in scoreboard packet data and displays it
/// </summary>
class Scoreboard : public IScoreboard
{
public:
	Scoreboard();
	~Scoreboard();

	void Read(InputMemoryBitStream& inInputStream, UINT8 playerCount);

	/// <summary>
	/// Draw a window containing all of the client scores
	/// </summary>
	void DrawUI() const;

private:

	/// <summary>
	/// A map of all the client names and their respective score
	/// </summary>
	std::unordered_map<std::string, int> clientScores;
};

