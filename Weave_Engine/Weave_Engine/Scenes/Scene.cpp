#include "stdafx.h"

#include "Scene.h"
#include "Resources/MeshRenderer.h"
#include "Lighting/PointLight.h"

using namespace SceneManagement;

Scene::Scene()
{
    EntityPool = new ObjectPool<Entity>( MAX_ENTITY_COUNT );

    EntityArray_Raw = EntityPool->GetRaw();
    // Initalize the entity array
    for ( size_t i = 0; i < MAX_ENTITY_COUNT; ++i )
    {
        EntityArray_Raw [ i ].SetIsValid( false );
    }
}

Scene::~Scene()
{
    UnloadAllEntities( true );
    UnloadAllLights();
    EntityArray_Raw = nullptr;
    SAFE_DELETE( EntityPool );
}

void SceneManagement::Scene::Read( InputMemoryBitStream & inInputStream )
{
    // Read in the array of entity info
    UINT32 numEntities = 0;
    inInputStream.Read( numEntities );

    std::unordered_map< INT32, IEntity* > entitiesToDestroy = NetworkIdToEntityMap;

    LOG_TRACE( "Num entities on server scene: {}", numEntities );
    for ( size_t i = 0; i < numEntities; ++i )
    {
        // Read in the ID and the action that this entity used
        INT32 networkID = 0;
        inInputStream.Read( networkID );
        EReplicationAction action = EReplicationAction::ERA_Update;
        inInputStream.Read( action, 2 );

		EReplicatedClassType aClassType = EReplicatedClassType::EObstacle_Class;
		inInputStream.Read( aClassType, 2 );

        switch ( action )
        {
        case ERA_Create:
        {
            LOG_ERROR( "CREATE THE ENTITY! " );
        }
        break;
        case ERA_Update:
        {
            // Do we have this in our replicated map
            if ( !IsObjectReplicated( networkID ) )
            {
				// If not, add it
				Entity* ent = AddEntity("Newly Added rep object");
				switch ( aClassType )
				{
				case ETank_Class:
				{
					LOG_WARN( "This is a tank!" );
					ent->SetName("New Tank");
					// #TODO Make entity creation and components replicated
					ent->AddComponent< MeshRenderer >( L"Assets/Materials/Cobblestone.wmat", L"Assets/Models/My_Tank.obj" );
					PointLightData lightData = {};
					lightData.Color = ( NetworkIdToEntityMap.size() ? glm::vec3( 1.f, 0.f, 0.f ) : glm::vec3( 0.f, 1.f, 0.f ) );
					lightData.Range = 8.f;
					lightData.Intensity = 10.f;
					ent->AddComponent< PointLight >(lightData, glm::vec3(0.f, 5.f, 0.f));
				}
				break;
				case EBullet_Class:
				{
                    LOG_WARN("This is a bullet!");
					ent->SetName("New Bullet");
					ent->AddComponent< MeshRenderer >( L"Assets/Materials/Cobblestone.wmat", L"Assets/Models/sphere.obj" );
				}
				break;
				case EObstacle_Class:
                    LOG_WARN("This is an obstacle!");

					break;
				default:
					break;
				}

                ent->SetNetworkID( networkID );
                AddReplicatedObject( ent );
            }
            IEntity* replicatedEnt = NetworkIdToEntityMap[ networkID ];
            assert( replicatedEnt != nullptr );
			replicatedEnt->SetReplicationClassType( aClassType );
            // Have this entity read in it's update data
            replicatedEnt->Read( inInputStream );
        }
        break;
        case ERA_Destroy:
        {
            // Remove this network ID from the scene
            RemoveReplicatedObject( networkID );
        }
        break;
        default:
            break;
        }
    }
}

// Entity -----------------------------------------------

Entity * Scene::AddEntity( std::string aName )
{
    Entity* newEnt = EntityPool->GetResource();

    assert( newEnt != nullptr );

    newEnt->SetName( aName );
    newEnt->SetIsActive( true );
    newEnt->SetIsValid( true );

    LOG_TRACE( "Add raw entity! {}", aName );

    return newEnt;
}

Entity * Scene::AddEntityFromfile( nlohmann::json const & aFile )
{
    Entity* newEnt = EntityPool->GetResource();

    assert( newEnt != nullptr );

    newEnt->SetIsValid( true );
    newEnt->ConstructFromFile( aFile );

    LOG_TRACE( "Add raw entity from file! {}", newEnt->GetName() );

    return newEnt;
}

void Scene::UnloadAllEntities( bool aOverrideDestroyOnLoad )
{
    // Delete each entity that has been added
    for ( size_t i = 0; i < MAX_ENTITY_COUNT; ++i )
    {
        if ( &EntityArray_Raw [ i ] != nullptr )
        {
            if ( EntityArray_Raw [ i ].GetIsValid() &&
                ( EntityArray_Raw [ i ].GetIsDestroyableOnLoad() ||
                    aOverrideDestroyOnLoad ) )
            {
                EntityArray_Raw [ i ].Reset();
                EntityPool->ReturnResource( i );
            }
        }
    }
}

void Scene::ResetScene()
{
    // Clears the known replicated objects
    IScene::ResetScene();

    UnloadAllEntities( false );
    UnloadAllLights();
    SceneName = "DEFAULT_SCENE";
    LOG_TRACE( "Reset Scene!" );
}

// Lighting ----------------------------------------------
void Scene::UnloadAllLights()
{
    // Release references to any info
    if ( DirLights.size() != 0 ) DirLights.clear();
    if ( PointLights.size() != 0 ) PointLights.clear();
}

void Scene::SetShaderInfo( SimpleVertexShader* aVertShader, SimplePixelShader* aPixShader )
{
    assert( aVertShader && aPixShader );
    // For each mesh renderer ...
    SetLightData( aPixShader );
}

void Scene::AddDirLight( DirLight * aDirLight )
{
    DirLights.push_back( aDirLight );
}

void Scene::AddPointLight( PointLight * aPointLight )
{
    PointLights.push_back( aPointLight );
}

void Scene::SetLightData( SimplePixelShader* aPixShader )
{
    // Send dir lights -------------------------------
    size_t dirLightCount = DirLights.size();

    if ( DirLights.size() > 0 )
    {
        // There needs to be a raw array to the dir light data that is continuous in memory
        // in order for the GPU to be able to read it
        DirectionalLightData dirlightData [ MAX_DIR_LIGHTS ];

        for ( size_t i = 0; i < DirLights.size(); ++i )
        {
            // Skip if this component is disabled
            if ( !DirLights [ i ]->IsEnabled() )
            {
                --dirLightCount;
                continue;
            }
            // Copy the lighting data to the raw array of lighting data
            memcpy(
                ( void* ) ( dirlightData + i ),
                ( ( void* ) ( &DirLights [ i ]->GetLightData() ) ),
                sizeof( DirectionalLightData )
            );
        }
        // Send data to the GPU
        aPixShader->SetData( "DirLights", ( void* ) ( &dirlightData [ 0 ] ), sizeof( DirectionalLightData ) * MAX_DIR_LIGHTS );
    }
    aPixShader->SetInt( "DirLightCount", static_cast< int >( dirLightCount ) );

    // Send point lights -------------------------------
    size_t pointLightCount = PointLights.size();

    if ( PointLights.size() > 0 )
    {
        PointLightData pointLightData [ MAX_POINT_LIGHTS ];

        for ( size_t i = 0; i < PointLights.size(); ++i )
        {
            // Skip if disabled
            if ( !PointLights [ i ]->IsEnabled() )
            {
                --pointLightCount;
                continue;
            }
            // Copy the point light data to the raw array 
            memcpy(
                ( void* ) ( pointLightData + i ),
                ( ( void* ) ( &PointLights [ i ]->GetLightData() ) ),
                sizeof( PointLightData )
            );
        }
        // Send the raw data to the GPU
        aPixShader->SetData( "PointLights", ( void* ) ( &pointLightData [ 0 ] ), sizeof( PointLightData ) * MAX_POINT_LIGHTS );
    }
    aPixShader->SetInt( "PointLightCount", static_cast< int >( pointLightCount ) );
}