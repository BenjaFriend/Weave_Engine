#include "../stdafx.h"

#include "ScriptUtils.h"

using namespace Scripting;

void Scripting::TestScripting()
{
    sol::state lua;
    lua.open_libraries( sol::lib::base );

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
                        LOG_WARN( "Entity data: {}", aEntity.name );
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

ScriptManager::ScriptManager( 
    ID3D11Device* aDevice, 
    ID3D11DeviceContext* aContext 
)
    : device( aDevice ), context( aContext )
{
    entityMan = EntityManager::GetInstance();
}

ScriptManager::~ScriptManager()
{
    updateTicks.clear();

    entityMan = nullptr;
}

void ScriptManager::Update( float deltaTime )
{
    for ( auto it : updateTicks )
    {
        it( deltaTime );
    }
}

void ScriptManager::LoadScripts()
{
    // #TODO 
    // Load all scripts currently here
    LoadScript( "Assets/Scripts/test.lua" );
}

void ScriptManager::LoadScript( const char * aFile )
{
    sol::state lua;
    lua.open_libraries( sol::lib::base );

    // Set lua types
    DefinedLuaTypes( lua );

    // Load in this script...
    lua.script_file( aFile );

    // Call start on the script if it has it
    sol::optional <sol::function> unsafe_startFunc = lua [ "start" ];
    if ( unsafe_startFunc != sol::nullopt )
    {
        sol::function& start_function = unsafe_startFunc.value();
        start_function();
    }

    // Store the update function for later if there is one
    sol::optional <sol::function> unsafe_updateFunc = lua [ "update" ];
    if ( unsafe_updateFunc != sol::nullopt )
    {
        sol::function& update_func = unsafe_updateFunc.value();

        updateTicks.emplace_back( update_func );
    }

    luaStates.push_back( std::move( lua ) );
}

void ScriptManager::DefinedLuaTypes( sol::state & aLua )
{
    aLua [ "device" ] = device;
    aLua [ "context" ] = context;

    // Define the entity types
    aLua.new_usertype<Scripting::EntityCreationData>( "Entity",

        sol::constructors<
        EntityCreationData( const char* aName, const char* aMeshName )
        >(),

        "name", &EntityCreationData::name,
        "meshName", &EntityCreationData::meshName,
        "SetPos", &EntityCreationData::SetPos
        );

    aLua.new_usertype<VEC3>( "VEC3",
        "x", &VEC3::x,
        "y", &VEC3::y,
        "z", &VEC3::z
        );
}
