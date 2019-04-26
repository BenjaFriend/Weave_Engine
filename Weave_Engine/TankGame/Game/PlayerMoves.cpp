#include "pch.h"
#include "PlayerMoves.h"

using namespace Tanks;

std::shared_ptr<PlayerMoves> PlayerMoves::Instance = nullptr;
Input::InputManager* PlayerMoves::inMan;

std::shared_ptr<PlayerMoves> Tanks::PlayerMoves::Init()
{
    assert( Instance == nullptr );

    Instance = std::make_shared<PlayerMoves>();

    using namespace Input;
    inMan = InputManager::GetInstance();
    PlayerMoves* inst = Instance.get();
    inMan->BindAction( inst, &Tanks::PlayerMoves::MoveLeft, InputType::Move_Left );
    inMan->BindAction( inst, &Tanks::PlayerMoves::MoveRight, InputType::Move_Right );
    inMan->BindAction( inst, &Tanks::PlayerMoves::MoveUp, InputType::Move_Up);
    inMan->BindAction( inst, &Tanks::PlayerMoves::MoveDown, InputType::Move_Down);
    inMan->BindAction( inst, &Tanks::PlayerMoves::OnFire, InputType::Fire );

    return Instance;
}

PlayerMoves::PlayerMoves()
{
}

PlayerMoves::~PlayerMoves()
{

}

void PlayerMoves::QueueCurrentMoves()
{
	for (auto& move : currentMoves)
	{
		MoveQueue.push_back(move);
		move.time = 0.0f;
	}
}

void PlayerMoves::Update(float deltaTime)
{
	std::vector<TimedMove>::iterator it = currentMoves.begin();

	while (it != currentMoves.end())
	{
		// Increase the amount of time that an input is held down
		if (inMan->IsInputTypeDown(it->inputType))
		{
			it->time += deltaTime;
			++it;
		}
		// Add the input to the move queue
		else
		{
			it->time += deltaTime;
			MoveQueue.push_back(*it);
			it = currentMoves.erase(it);
		}
	}
}