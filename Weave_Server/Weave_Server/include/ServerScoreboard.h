#pragma once
#include "../Weave_Common/Networking/IScoreboard.h"
#include "ClientProxy.h"


/// <summary>
/// Server scoreboard controls all client scores
/// Updating sending scores to the state packer buffer. 
/// </summary>
class ServerScoreboard : public IScoreboard
{
public:

	ServerScoreboard();
	~ServerScoreboard();

	int GetClientScore(ClientProxyPtr client);

	void SetClientScore(ClientProxyPtr client, int score);
	void AddScoreToClient(ClientProxyPtr client, int score);

	/// <summary>
	/// Remove a client from the scoreboard map 
	/// </summary>
	void RemoveClient(ClientProxyPtr client);

	void Write(OutputMemoryBitStream& inOutputStream) const;

private:

	/// <summary>
	/// Map of all tracked clients and their respective score
	/// </summary>
	std::unordered_map<ClientProxyPtr, int> clientScores;
};

