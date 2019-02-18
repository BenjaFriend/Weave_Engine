#include "../stdafx.h"

#include "ScriptUtils.h"

using namespace Scripting;

ScriptManager::ScriptManager( )
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
    aLua.set_function( "LoadMaterial", &Scripting::ScriptManager::LoadMaterial, this );
    aLua.set_function( "CreateEntity", &Scripting::ScriptManager::CreateEntity, this );

    // Define the entity types
    aLua.new_usertype<Material>( "Material" );

    aLua.new_usertype<glm::vec3>( "VEC3",
        sol::constructors<glm::vec3( float x, float y, float z )>(),
        "x", &glm::vec3::x,
        "y", &glm::vec3::y,
        "z", &glm::vec3::z
        );

    aLua.new_usertype<Transform>( "Transform",
        "GetPosition", &Transform::GetPosition,
        "SetPosX", &Transform::SetPosX,
        "SetPosY", &Transform::SetPosY,
        "SetPosZ", &Transform::SetPosZ,
        "GetScale", &Transform::GetScale,
        "SetScaleX", &Transform::SetScaleX,
        "SetScaleY", &Transform::SetScaleY,
        "SetScaleZ", &Transform::SetScaleZ
        );

    aLua.new_usertype<Entity>( "Entity",
        "GetName", &Entity::GetName,
        "SetName", &Entity::SetName,
        "SetIsActive", &Entity::SetIsActive,
        "GetIsActive", &Entity::GetIsActive,
        "GetTransform", &Entity::GetTransform
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
    sol::optional <sol::function> unsafe_Func = lua [ aFuncName ];
    if ( unsafe_Func != sol::nullopt )
    {
        // Run that function
        unsafe_Func.value()( );
    }
}

/** Called from Lua */
Material* ScriptManager::LoadMaterial( const sol::table & aMatInfo )
{
    Material_ID name = aMatInfo [ "name" ];
    FileName vsName = aMatInfo [ "VS" ];
    FileName psName = aMatInfo [ "PS" ];
    FileName albedo = aMatInfo [ "albedo" ];
    FileName norm = aMatInfo [ "norm" ];
    FileName roughness = aMatInfo [ "roughness" ];
    FileName metal = aMatInfo [ "metal" ];

    ResourceManager* resourceMan = ResourceManager::GetInstance();

    SimpleVertexShader* vs = resourceMan->LoadShader<SimpleVertexShader>(
        vsName );

    SimplePixelShader* ps = resourceMan->LoadShader<SimplePixelShader>(
        psName );

    assert( ps && vs );

    return resourceMan->LoadMaterial(
        name,
        vs,
        ps,
        albedo,
        norm,
        roughness,
        metal,
        0 );   // Use default sampler
}

/** Called from Lua */
Entity* ScriptManager::CreateEntity( const sol::table & aEntityInfo )
{
    std::string name = aEntityInfo [ "name" ];
    FileName meshName = aEntityInfo [ "mesh" ];
    Material* mat = aEntityInfo [ "material" ];
    glm::vec3 pos = {};
    sol::optional<glm::vec3> unsafe_pos = aEntityInfo [ "pos" ];
    if ( unsafe_pos != sol::nullopt )
    {
        pos = unsafe_pos.value();
    }

    ResourceManager* resMan = ResourceManager::GetInstance();
    Mesh* mesh = resMan->LoadMesh( meshName );
    
    // Create the entity in the entity manager
    Entity* ent = 
        SceneManagement::SceneManager::GetInstance()->GetActiveScene()->AddEntity( name, pos );

    ent->AddComponent<MeshRenderer>( mat, mesh );
    return nullptr;
}