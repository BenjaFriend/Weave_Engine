#include "Scoreboard.h"


Scoreboard::Scoreboard()
{
}


Scoreboard::~Scoreboard()
{
}

void Scoreboard::DrawUI(Entity* entityArray)
{
	ImGui::Begin("Scoreboard");

	for (size_t i = 0; i < MAX_ENTITY_COUNT; ++i)
	{
		Entity* CurrentEntity = &entityArray[i];
		if (CurrentEntity == nullptr || !CurrentEntity->GetIsActive() ||
			!CurrentEntity->GetIsInUse() || CurrentEntity->GetReplicationClassType() != EReplicatedClassType::ETank_Class)
			continue;

		std::string entityID = CurrentEntity->GetName();
		std::string entityScore = std::to_string(CurrentEntity->GetScore());
		std::string scoreText = "Player " + entityID + ": " + entityScore;

		ImGui::Text(scoreText.c_str(), ImVec2(ImGui::GetWindowWidth(), 0.f));
	}

	ImGui::End();
}