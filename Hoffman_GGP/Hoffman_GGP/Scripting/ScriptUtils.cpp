#include "../stdafx.h"

#include "ScriptUtils.h"

using namespace Scripting;

ScriptManager::ScriptManager( ID3D11Device* aDevice, ID3D11DeviceContext* aContext )
    : Device( aDevice ), Context( aContext )
{
}

ScriptManager::~ScriptManager()
{
    UpdateTicks.clear();
    LuaStates.clear();
}

void ScriptManager::Update( float deltaTime )
{
    for ( auto it : UpdateTicks )
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

        UpdateTicks.emplace_back( update_func );
    }

    LuaStates.push_back( std::move( lua ) );
}

void ScriptManager::DefinedLuaTypes( sol::state & aLua )
{
    aLua [ "device" ] = Device;
    aLua [ "context" ] = Context;

    // Define the entity types
    aLua.new_usertype<EntityCreationData>( "Entity",
        sol::constructors<
        EntityCreationData( std::string aName, FileName aMeshName, MaterialCreationData* matData )
        >(),

        "SetPos", &EntityCreationData::SetPos,

        "SetScale", &EntityCreationData::SetScale,

        "SetMass", &EntityCreationData::SetMass,
        "GetMass", &EntityCreationData::GetMass

        );

    aLua.new_usertype<MaterialCreationData>( "Material",

        sol::constructors<
        MaterialCreationData(
            ID3D11Device* aDevice,
            ID3D11DeviceContext* aContext,
            FileName vertexShader,
            FileName pixelShader,
            FileName albedoTexture,
            FileName normalTexture,
            FileName roughnessTexture,
            FileName metalTexture
        )>()

        );

    aLua.new_usertype<VEC3>( "VEC3",
        "x", &VEC3::x,
        "y", &VEC3::y,
        "z", &VEC3::z
        );
}
