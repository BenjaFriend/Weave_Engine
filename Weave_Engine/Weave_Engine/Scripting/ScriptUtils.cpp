#include "../stdafx.h"

#include "ScriptUtils.h"
#include "../Input/InputManager.h"
#include "../ECS/ComponentManager.h"
#include "../Scenes/SceneManager.h"

using namespace Scripting;

ScriptManager::ScriptManager()
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
    aLua.set_function( "Print", &Scripting::ScriptManager::Log_Print, this );

    ResourceManager * resMan = ResourceManager::GetInstance();
    aLua.set_function( "LoadMaterial", &ResourceManager::LoadMaterial, resMan );
    aLua.set_function( "CreateEntity", &Scripting::ScriptManager::CreateEntity, this );

    Input::InputManager* inpMan = Input::InputManager::GetInstance();
	aLua.set_function("IsIKeyDown", &Input::InputManager::IsKeyDown, inpMan);
	aLua.set_function( "IsCKeyDown", &Input::InputManager::IsCKeyDown, inpMan );
	aLua.set_function("GetMousePosition", &Input::InputManager::GetMousePosition, inpMan);
    aLua.set_function( "MoveCamera", &Scripting::ScriptManager::MoveCamera, this );

    // Define the entity types
    aLua.new_usertype<Material>( "Material" );

	aLua.new_usertype<glm::vec2>("VEC2",
		sol::constructors<glm::vec2(float x, float y)>(),
		"x", &glm::vec2::x,
		"y", &glm::vec2::y
		);

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
Entity* ScriptManager::CreateEntity( const sol::table & aEntityInfo )
{
    std::string name = "DEFAULT LUA NAME -- ";

    sol::optional<std::string> unsafe_entName = aEntityInfo [ "name" ];
    if ( unsafe_entName != sol::nullopt )
    {
        name = unsafe_entName.value();
    }

    // Load in Mesh name
    FileName meshName = {};
    sol::optional<FileName> unsafe_meshName = aEntityInfo [ "mesh" ];
    if ( unsafe_meshName != sol::nullopt )
    {
        meshName = unsafe_meshName.value();
    }

    // Load in material
    Material* mat = nullptr;
    sol::optional<Material*> unsafe_mat = aEntityInfo [ "material" ];
    if ( unsafe_mat != sol::nullopt )
    {
        mat = unsafe_mat.value();
    }

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
        SceneManagement::SceneManager::GetInstance()->GetActiveScene()->AddEntity( name );
    ent->GetTransform()->SetPosition( pos );

    if ( mat != nullptr && mesh != nullptr )
    {
        ent->AddComponent<MeshRenderer>( mat, mesh );
    }
    return ent;
}

void ScriptManager::Log_Print( std::string msg )
{
    LOG_TRACE( "{}", msg );
}

void ScriptManager::MoveCamera(glm::vec3 move, glm::vec2 mouseMove)
{

	Camera* cam = CameraManager::GetInstance()->GetActiveCamera();
	assert(cam != nullptr);
	Transform* transform = cam->GetEntity()->GetTransform();

	transform->Rotate(glm::vec3(mouseMove.y, mouseMove.x, 0));
	transform->MoveRelative(move.x, move.y, move.z);
	cam->SetPosition(transform->GetPosition());
}