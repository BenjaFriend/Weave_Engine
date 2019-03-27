#pragma once

#include "Input/InputManager.h"
#include <deque>
#include <memory>

namespace Tanks
{
    class PlayerMoves
    {
    public:

        static std::shared_ptr<PlayerMoves> Init();

        static std::shared_ptr<PlayerMoves> Instance;
        PlayerMoves();

        ~PlayerMoves();
        /// <summary>
        /// Clear all moves of the player
        /// </summary>
        void Clear() { MoveQueue.clear(); }

        /// <summary>
        /// Check if the player has put any moves into the queue
        /// </summary>
        /// <returns>True if the queue of moves is not empty</returns>
        bool HasMoves() { return !MoveQueue.empty(); }

		void UpdateMoveQueue();

        const std::deque<Input::InputType> & GetMoveQueue() const { return MoveQueue; }

        void MoveLeft() { LOG_TRACE("Action"); MoveQueue.push_back( Input::InputType::Move_Left ); }

        void MoveRight() { MoveQueue.push_back( Input::InputType::Move_Right ); }
        
        void MoveDown() { MoveQueue.push_back( Input::InputType::Move_Down ); }

        void MoveUp() { MoveQueue.push_back( Input::InputType::Move_Up ); }

        void OnFire() { MoveQueue.push_back( Input::InputType::Fire ); }

    private:

        // Keep track of what buttons have been pressed
        std::deque<Input::InputType> MoveQueue;

		std::unordered_map<Input::InputType, float> inputTime;

    };
}   // namespace Tanks