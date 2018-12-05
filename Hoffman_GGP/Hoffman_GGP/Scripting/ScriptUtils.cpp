#include "../stdafx.h"

#include "ScriptUtils.h"

void Scripting::TestScripting()
{
    sol::state lua;
    lua.open_libraries( sol::lib::base );

    struct EntityCreationData
    {
        int data;
        std::string file;
    };

    lua.new_usertype<EntityCreationData>( "EntityCreationData" );


    lua.script_file( "Assets/Scripts/test.lua" );

    sol::optional<sol::table> maybeObjs = lua[ "objs" ];
    if ( maybeObjs != sol::nullopt )
    {
        sol::table& objsTable = maybeObjs.value();

        std::vector<std::reference_wrapper<EntityCreationData>> entitiesToCreate;

        auto parseFunc = [ &entitiesToCreate ] ( auto& func, sol::table & aTable ) -> void
        {
            // Iterate over this table
            for ( auto key_val : aTable )
            {
                const sol::object& key = key_val.first;
                const sol::object& value = key_val.second;

                LOG_TRACE( "Key: {}", key.as<std::string>() );

                // Determine the type of data that is in this key
                auto t = value.get_type();
                switch ( t )
                {
                case sol::type::table:
                    // Check for other components types
                    func( func, value.as<sol::table>() );
                    break;

                case sol::type::string:
                    // What is this key? 
                    // Do something with that value
                    //aTable[ key.as<std::string>() ];
                    break;
                    //case sol::type::number:
                    //case sol::type::boolean:
                    //case sol::type::function:
                case sol::type::userdata:

                    //sol::optional<EntityCreationData&> possibleEntity = sol::nullopt;

                    //possibleEntity = value.as<sol::optional<EntityCreationData&>>();
                    //if ( possibleEntity )
                    //{
                       // EntityCreationData& aEntity = possibleEntity.value();
                        //entitiesToCreate.push_back( aEntity );
                    //}


                    break;
                    
                case sol::type::none:
                case sol::type::lua_nil:
                default:
                    break;
                }
            }
        };

        parseFunc( parseFunc, objsTable );
    }
}
