#include "Scoreboard.h"


Scoreboard::Scoreboard()
{
}


Scoreboard::~Scoreboard()
{
}

void Scoreboard::Read(InputMemoryBitStream & inInputStream, UINT8 playerCount)
{
	clientScores.clear();

	for (int i = 0; i < playerCount; ++i)
	{
		std::string name;
		int score;
		inInputStream.Read(name);
		inInputStream.Read(score);

		clientScores[name] = score;
	}
}

void Scoreboard::Write(OutputMemoryBitStream & inOutputStream) const
{
}

void Scoreboard::DrawUI() const
{
	ImGui::Begin("Scoreboard");

	auto it = clientScores.begin();

	while (it != clientScores.end())
	{
		std::string entityID = it->first;
		std::string entityScore = std::to_string(it->second);
		std::string scoreText = "Player " + entityID + ": " + entityScore;
		ImGui::Text(scoreText.c_str(), ImVec2(ImGui::GetWindowWidth(), 0.f));

		it++;
	}

	ImGui::End();
}
