#pragma once

#include "stdafx.h"

namespace Input
{

    enum class InputType : UINT8
    {
        NONE,
        Move_Left,
        Move_Right,
        Move_Up,
        Move_Down,
        Horizontal,
        Vertical,

        Fire,
        FireReleased,

        Look,
        LookReleased,

        Use,

        Quit
    };
}
