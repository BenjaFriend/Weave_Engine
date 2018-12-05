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


/// <summary>
///  https://github.com/ThePhD/sol2/blob/develop/examples/optional_with_iteration.cpp
/// </summary>
void IterateOverTestKeys()
{
    struct thing
    {
        int a = 20;

        thing() = default;
        thing( int a ) : a( a ) {}
    };

    struct super_thing : thing
    {
        int b = 40;
    };

    struct unrelated
    {

    };

    sol::state lua;

    // Comment out the new_usertype call
    // to prevent derived class "super_thing"
    // from being picked up and cast to its base
    // class
    lua.new_usertype<super_thing>( "super_thing",
        sol::base_classes, sol::bases<thing>()
        );

    // Make a few things
    lua[ "t1" ] = thing{};
    lua[ "t2" ] = super_thing{};
    lua[ "t3" ] = unrelated{};
    // And a table
    lua[ "container" ] = lua.create_table_with(
        0, thing{ 50 },
        1, unrelated{},
        4, super_thing{}
    );

    lua.script_file( "Assets/Scripts/test.lua" );

    std::vector<std::reference_wrapper<thing>> things;

    auto fx = [ &things ] ( auto& f, auto& tbl ) -> void
    {
        for ( auto key_value_pair : tbl )
        {
            const sol::object& key = key_value_pair.first;
            const sol::object& value = key_value_pair.second;
            sol::type t = value.get_type();
            switch ( t )
            {
            case sol::type::table:
            {
                sol::table inner = value.as<sol::table>();
                f( f, inner );
            }
            break;
            case sol::type::userdata:
            {
                // This allows us to check if a userdata is 
                // a specific class type
                sol::optional<thing&> maybe_thing = value.as<sol::optional<thing&>>();
                if ( maybe_thing )
                {
                    thing& the_thing = maybe_thing.value();
                    if ( key.is<std::string>() )
                    {
                        LOG_TRACE( "Key : {}", key.as<std::string>() );
                    }
                    else if ( key.is<int>() )
                    {
                        std::cout << "key " << key.as<int>() << " is a thing -- ";
                    }
                    std::cout << "thing.a ==" << the_thing.a << std::endl;
                    things.push_back( the_thing );
                }
            }
            break;
            default:
                break;
            }
        }   // itr through map
    };  // fx

    fx( fx, lua );
}