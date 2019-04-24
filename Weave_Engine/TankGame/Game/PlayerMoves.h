#pragma once

#include "Input/InputManager.h"
#include <deque>
#include <memory>

namespace Tanks
{
    class PlayerMoves
    {
    public:

		struct TimedMove
		{
			Input::InputType inputType;
			float time;
		};

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

        const std::deque<TimedMove> & GetMoveQueue() const { return MoveQueue; }

		void MoveLeft() { currentMoves.push_back({ Input::InputType::Move_Left, 0 }); }

		void MoveRight() { currentMoves.push_back({ Input::InputType::Move_Right, 0 }); }
        
		void MoveDown() { currentMoves.push_back({ Input::InputType::Move_Down, 0 }); }

		void MoveUp() { currentMoves.push_back({ Input::InputType::Move_Up, 0 }); }

		void OnFire() { currentMoves.push_back({ Input::InputType::Fire, 0 }); }

		void QueueCurrentMoves();

		void Update(float deltaTime);

    private:

		static Input::InputManager* inMan;

        // Keep track of what buttons have been pressed
        std::deque<TimedMove> MoveQueue;
		std::vector<TimedMove> currentMoves;

    };
}   // namespace Tanks