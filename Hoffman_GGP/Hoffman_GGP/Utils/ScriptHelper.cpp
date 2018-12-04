#include "../stdafx.h"

#include "ScriptHelper.h"

bool luah::loadScript( lua_State* L, const std::string& filename )
{
    if ( luaL_loadfile( L, filename.c_str() ) || lua_pcall( L, 0, 0, 0 ) )
    {
        LOG_ERROR( "Failed to load lua script: {}", filename );
        return false;
    }
    else
    {
        LOG_TRACE( "Loaded lua script: {}", filename );
        return true;
    }
}

void luah::loadGetKeysFunction( lua_State* L )
{
    std::string code =
        R"(function getKeys(t)
        s = {}
        for k, v in pairs(t) do
            table.insert(s, k)
            end 
        return s 
        end)";
    luaL_dostring( L, code.c_str() );
}

std::vector<std::string> luah::getTableKeys( lua_State* L, const std::string& name )
{
    lua_gettostack( L, "getKeys" ); // get function
    if ( lua_isnil( L, -1 ) )
    {
        LOG_WARN( "Get keys function is not loaded. Loading..." );
        loadGetKeysFunction( L );
        lua_gettostack( L, "getKeys" );
    }
    lua_pcall( L, 1, 1, 0 ); // execute getKeys function.

    // One parameter, one return
    std::vector<std::string> keys;
    while ( lua_next( L, -2 ) ) // get values one by one
    { 
        if ( lua_type( L, -1 ) == LUA_TSTRING ) // check if key is a string
        { 
            keys.push_back( lua_tostring( L, -1 ) );
        }
        lua_pop( L, 1 );
    }

    lua_settop( L, 0 ); // remove s table from stack 
    return keys;
}

void luah::lua_gettostack( lua_State* L, const std::string& variableName )
{
    int level = 0;
    std::string var = "";
    for ( unsigned int i = 0; i < variableName.size(); i++ )
    {
        if ( variableName.at( i ) == '.' )
        {
            if ( level == 0 )
            {
                lua_getglobal( L, var.c_str() );
            }
            else
            {
                lua_getfield( L, -1, var.c_str() );
            }

            if ( lua_isnil( L, -1 ) )
            {
                LOG_ERROR( "Error, can't get {}", variableName );
                return;
            }
            else
            {
                var = "";
                level++;
            }
        }
        else
        {
            var += variableName.at( i );
        }
    }
    if ( level == 0 )
    {
        lua_getglobal( L, var.c_str() );
    }
    else
    {
        lua_getfield( L, -1, var.c_str() );
    }

    if ( level == 0 ) { return; } // no need to remove anything

    int tableIndex = lua_gettop( L ) - level;
    lua_replace( L, tableIndex );
    lua_settop( L, tableIndex );
}