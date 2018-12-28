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
    OnClickCallbacks.clear();
    LuaStates.clear();
}

void ScriptManager::Update( float deltaTime )
{
    for ( auto it : UpdateTicks )
    {
        it( deltaTime );
    }
}

void Scripting::ScriptManager::OnClick()
{
    for ( auto it : OnClickCallbacks )
        it();
    LOG_TRACE( "Script click callback!" );
}

void ScriptManager::LoadScripts()
{
    ReadDirectory( "Assets/Scripts/", ScriptPaths );

    for ( auto it : ScriptPaths )
    {
        LoadScript( ( it ).c_str() );
    }
}

void ScriptManager::LoadScript( const char * aFile )
{
    sol::state lua;
    lua.open_libraries( sol::lib::base );

    // Set lua types
    DefinedLuaTypes( lua );

    // Load in this script...
    lua.script_file( aFile );

    RunLuaFunction( lua, "start" );

    AddCallback( lua, "update", UpdateTicks );
    AddCallback( lua, "onClick", OnClickCallbacks );


    LuaStates.push_back( std::move( lua ) );
    LOG_TRACE( "Loaded Lua script: {}", aFile );
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
        "SetScale", &EntityCreationData::SetScale
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

void ScriptManager::ReadDirectory( 
    const std::string & dirName,
    std::vector<std::string>& aPathVec )
{
    namespace fs = std::filesystem;

    for ( const auto & entry : fs::directory_iterator( dirName ) )
    {
        aPathVec.push_back( entry.path().generic_string() );
    }
}

void Scripting::ScriptManager::AddCallback( 
    const sol::state & lua,
    const char * aFuncName,
    std::vector<sol::function>& aCallbackVec )
{
    // Store the function for later if there is one
    sol::optional <sol::function> unsafe_func = lua [ aFuncName ];
    if ( unsafe_func != sol::nullopt )
    {
        sol::function& safe_func = unsafe_func.value();
        aCallbackVec.emplace_back( safe_func );
    }
}

void Scripting::ScriptManager::RunLuaFunction( const sol::state & lua, const char * aFuncName )
{
    sol::optional <sol::function> unsafe_Func = lua [ "start" ];
    if ( unsafe_Func != sol::nullopt )
    {
        // Run that function
        unsafe_Func.value()();
    }
}
