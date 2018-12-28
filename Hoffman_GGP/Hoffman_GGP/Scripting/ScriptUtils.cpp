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
    DefineLuaTypes( lua );

    // Load in this script...
    lua.script_file( aFile );

    RunLuaFunction( lua, "start" );

    AddCallback( lua, "update", UpdateTicks );
    AddCallback( lua, "onClick", OnClickCallbacks );

    LuaStates.push_back( std::move( lua ) );
    LOG_TRACE( "Loaded Lua script: {}", aFile );
}

void ScriptManager::DefineLuaTypes( sol::state & aLua )
{
    aLua [ "device" ] = Device;
    aLua [ "context" ] = Context;
    aLua.set_function( "LoadMaterial", &Scripting::ScriptManager::LoadMaterial, this );
    aLua.set_function( "CreateEntity", &Scripting::ScriptManager::CreateEntity, this );

    // Define the entity types
    aLua.new_usertype<Material>( "Material" );

    aLua.new_usertype<VEC3>( "VEC3",
        sol::constructors<VEC3( float x, float y, float z )>(),
        "x", &VEC3::x,
        "y", &VEC3::y,
        "z", &VEC3::z
        );

    aLua.new_usertype<Entity>( "Entity",
        "GetName", &Entity::GetName,
        "SetName", &Entity::SetName,
        "SetIsActive", &Entity::SetIsActive,
        "GetIsActive", &Entity::GetIsActive,
        "GetPosition", &Entity::GetPosition,
        "SetPosX", &Entity::SetPosX,
        "SetPosY", &Entity::SetPosY,
        "SetPosZ", &Entity::SetPosZ,
        "GetScale", &Entity::GetScale,
        "SetScaleX", &Entity::SetScaleX,
        "SetScaleY", &Entity::SetScaleY,
        "SetScaleZ", &Entity::SetScaleZ
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

void ScriptManager::AddCallback(
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

void ScriptManager::RunLuaFunction(
    const sol::state & lua,
    const char * aFuncName )
{
    sol::optional <sol::function> unsafe_Func = lua [ "start" ];
    if ( unsafe_Func != sol::nullopt )
    {
        // Run that function
        unsafe_Func.value()( );
    }
}

/** Called from Lua */
Material* ScriptManager::LoadMaterial( const sol::table & aMatInfo )
{
    FileName vsName = aMatInfo [ "VS" ];
    FileName psName = aMatInfo [ "PS" ];
    FileName albedo = aMatInfo [ "albedo" ];
    FileName norm = aMatInfo [ "norm" ];
    FileName roughness = aMatInfo [ "roughness" ];
    FileName metal = aMatInfo [ "metal" ];

    ResourceManager* resourceMan = ResourceManager::GetInstance();

    SimpleVertexShader* vs = resourceMan->LoadShader<SimpleVertexShader>(
        Device,
        Context,
        vsName );

    SimplePixelShader* ps = resourceMan->LoadShader<SimplePixelShader>(
        Device,
        Context,
        psName );

    assert( ps && vs );

    SRV_ID dif = resourceMan->LoadSRV( Context, albedo );
    SRV_ID normSRV = resourceMan->LoadSRV( Context, norm );
    SRV_ID roughnessMap = resourceMan->LoadSRV( Context, roughness );
    SRV_ID metalMap = resourceMan->LoadSRV( Context, metal );

    Material_ID matID = resourceMan->LoadMaterial(
        vs,
        ps,
        dif,
        normSRV,
        roughnessMap,
        metalMap,
        0 );   // Use default sampler

    LOG_TRACE( "Load material called!" );

    return resourceMan->GetMaterial( matID );
}

/** Called from Lua */
Entity* ScriptManager::CreateEntity( const sol::table & aEntityInfo )
{
    std::string name = aEntityInfo [ "name" ];
    FileName meshName = aEntityInfo [ "mesh" ];
    Material* mat = aEntityInfo [ "material" ];
    VEC3 pos = {};
    sol::optional<VEC3> unsafe_pos = aEntityInfo [ "pos" ];
    if ( unsafe_pos != sol::nullopt )
    {

        pos = unsafe_pos.value();
    }

    ResourceManager* resMan = ResourceManager::GetInstance();
    Mesh* mesh = std::get<1>( resMan->LoadMesh( meshName ) );

    // Create the entity in the entity manager
    Entity_ID id = EntityManager::GetInstance()->AddEntity(
        mesh,
        mat,
        pos,
        name
    );
    LOG_TRACE( "Create entity {}", name );
    return EntityManager::GetInstance()->GetEntity( id );
}