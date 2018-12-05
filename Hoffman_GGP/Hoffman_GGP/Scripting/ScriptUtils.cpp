#include "../stdafx.h"

#include "ScriptUtils.h"

void Scripting::TestScripting()
{

    struct EntityData
    {
        std::string name;
        int hp;
    };

    sol::state lua;
    lua.open_libraries( sol::lib::base );
    lua.new_usertype<EntityData>(
        "EntityData",
        "name", &EntityData::name,
        "hp", &EntityData::hp
        );

    lua.script_file( "Assets/Scripts/test.lua" );

    sol::optional<sol::table> maybeObjs = lua[ "objs" ];
    if ( maybeObjs != sol::nullopt )
    {    
        sol::table& objsTable = maybeObjs.value();
        ParseTable( objsTable );
    }
}

void Scripting::ParseTable( sol::table & aTable )
{
    for ( auto key_val : aTable )
    {
        const sol::object& key = key_val.first;
        const sol::object& value = key_val.second;

        LOG_TRACE( "Key: {}", key.as<std::string>() );

        auto t = value.get_type();

        switch ( t )
        {
        case sol::type::none:
            break;
        case sol::type::lua_nil:
            break;

        case sol::type::table:
            // Check for other components types
            ParseTable( value.as<sol::table>() );
            break;

        case sol::type::string:
            break;
        case sol::type::number:
            break;
        case sol::type::thread:
            break;
        case sol::type::boolean:
            break;
        case sol::type::function:
            break;
        case sol::type::userdata:
            break;
        case sol::type::lightuserdata:
            break;
        case sol::type::poly:
            break;
        default:
            break;
        }

        //( *aTable )[ key.as<std::string>() ];
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