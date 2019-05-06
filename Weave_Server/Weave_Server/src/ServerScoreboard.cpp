#include "ServerScoreboard.h"



ServerScoreboard::ServerScoreboard()
{
}


ServerScoreboard::~ServerScoreboard()
{
	clientScores.clear();
}

int ServerScoreboard::GetClientScore(ClientProxyPtr client)
{
	return clientScores[client];
}

void ServerScoreboard::SetClientScore(ClientProxyPtr client, int score)
{
	clientScores[client] = score;
}

void ServerScoreboard::AddScoreToClient(ClientProxyPtr client, int score)
{
	clientScores[client] += score;
}

void ServerScoreboard::RemoveClient(ClientProxyPtr client)
{
	if (clientScores.find(client) != clientScores.end())
	{
		clientScores.erase(client);
	}
}

void ServerScoreboard::Write(OutputMemoryBitStream & inOutputStream) const
{
	auto it = clientScores.begin();

	while (it != clientScores.end())
	{
		inOutputStream.Write(it->first->GetName());
		inOutputStream.Write(it->second);

		it++;
	}
}
