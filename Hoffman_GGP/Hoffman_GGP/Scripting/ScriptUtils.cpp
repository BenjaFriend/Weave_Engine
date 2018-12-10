#include "../stdafx.h"

#include "ScriptUtils.h"
#include "../Entity/EntityManager.h"

using namespace Scripting;


void CreateEntity( EntityCreationData& aEntity )
{
    LOG_ERROR( "Create ent called! {}", aEntity.data );

}

void Scripting::TestScripting()
{
    sol::state lua;
    lua.open_libraries( sol::lib::base );

    // Define the entity types
    lua.new_usertype<EntityCreationData>( "Entity",
        "data", &EntityCreationData::data,
        "file", &EntityCreationData::file
        );

    lua.set_function( "CreateEntity", &CreateEntity );

    // Load in this script...
    lua.script_file( "Assets/Scripts/test.lua" );

    // Call start on the script if it has it
    sol::optional <sol::function> unsafe_startFunc = lua [ "start" ];
    if ( unsafe_startFunc != sol::nullopt )
    {
        sol::function& start_function = unsafe_startFunc.value();
        start_function();
        LOG_TRACE( "Has start function" );
    }



    // The object creation table
    sol::optional<sol::table> maybeObjs = lua [ "objs" ];

    // A vector of data to create
    std::vector<std::reference_wrapper<EntityCreationData>> entitiesToCreate;

    if ( maybeObjs != sol::nullopt )
    {
        sol::table& objsTable = maybeObjs.value();


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
                    LOG_TRACE( "String data Key: {}", key.as<std::string>() );
                    break;

                case sol::type::userdata:
                {
                    LOG_TRACE( "USER DATA KEY: {}", key.as<std::string>() );

                    sol::optional<EntityCreationData&> possibleEntity = value.as<sol::optional<EntityCreationData&>>();
                    if ( possibleEntity )
                    {
                        EntityCreationData& aEntity = possibleEntity.value();
                        LOG_WARN( "Entity data: {}", aEntity.data );
                        entitiesToCreate.push_back( aEntity );
                    }
                }
                break;

                case sol::type::function:
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
