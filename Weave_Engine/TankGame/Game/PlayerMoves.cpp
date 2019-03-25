#include "pch.h"
#include "PlayerMoves.h"

using namespace Tanks;

std::shared_ptr<PlayerMoves> PlayerMoves::Instance = nullptr;

std::shared_ptr<PlayerMoves> Tanks::PlayerMoves::Init()
{
    assert( Instance == nullptr );

    Instance = std::make_shared<PlayerMoves>();

    using namespace Input;
    InputManager* inMan = InputManager::GetInstance();
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