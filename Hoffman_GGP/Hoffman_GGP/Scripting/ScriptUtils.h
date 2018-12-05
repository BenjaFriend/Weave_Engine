#pragma once

#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>

namespace Scripting
{

    void TestScripting();

    void ParseTable( sol::table & aTable );

}
