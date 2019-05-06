#pragma once
#include "../Weave_Common/Networking/IScoreboard.h"
#include "ClientProxy.h"

class ServerScoreboard : public IScoreboard
{
public:

	ServerScoreboard();
	~ServerScoreboard();

	int GetClientScore(ClientProxyPtr client);

	void SetClientScore(ClientProxyPtr client, int score);
	void AddScoreToClient(ClientProxyPtr client, int score);

	void RemoveClient(ClientProxyPtr client);

	void Write(OutputMemoryBitStream& inOutputStream) const;

private:

	std::unordered_map<ClientProxyPtr, int> clientScores;
};

