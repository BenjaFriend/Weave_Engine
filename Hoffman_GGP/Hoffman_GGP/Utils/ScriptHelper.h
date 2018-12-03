/// <summary>
/// Helper functions for getting lua states
/// From this blog post: 
/// https://eliasdaler.wordpress.com/2015/09/08/using-lua-with-cpp-in-practice-part-2/
/// </summary>

#pragma once

#include "../stdafx.h"
#include <string>
#include <vector>

extern "C"
{
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
}
#include "LuaBridge/LuaBridge.h"

namespace luah
{

    /// <summary>
    /// Loads the lua given lua script. 
    /// </summary>
    /// <return>True if loading successful, false if failed.</return>
    bool loadScript( lua_State* L, const std::string& filename );

    void loadGetKeysFunction( lua_State* L );

    void lua_gettostack( lua_State* L, const std::string& variableName );

    std::vector<std::string> getTableKeys( lua_State* L, const std::string& name );

}   // namespace luah