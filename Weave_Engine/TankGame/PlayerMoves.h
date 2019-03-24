#pragma once

#include "Input/InputManager.h"
#include <deque>

class PlayerMoves
{
    

public:

    PlayerMoves();

    ~PlayerMoves();

    static PlayerMoves * Instance;

    void Clear() {  }

private:

    // Keep track of what buttons have been pressed
    std::deque<UINT8> MoveQueue;

};
