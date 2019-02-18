#include "../stdafx.h"

#include "Scene.h"
#include "../Entity/Entity.h"
using namespace SceneManagement;

Scene::Scene()
{ 
}

Scene::~Scene()
{
    UnloadAllEntities();
    UnloadAllLights();
}

// Entity -----------------------------------------------
Entity * Scene::AddEntity( std::string aName )
{
    Entity* tempEnt = new Entity( aName );
    EntityArray.push_back( tempEnt );
    return tempEnt;
}

Entity * Scene::AddEntity( std::string aName, glm::vec3 aPos )
{
    Entity* tempEnt = new Entity( aName, aPos );

    EntityArray.push_back( tempEnt );
    return tempEnt;
}

Entity * Scene::AddEntityFromfile( nlohmann::json const & aFile )
{
    Entity* tempEnt = new Entity( aFile );
    EntityArray.push_back( tempEnt );
    return tempEnt;
}

void Scene::DeleteEntity( Entity * aEntity )
{
    if ( aEntity == nullptr ) return;
    // Remove this entity from the vector
    for ( size_t i = 0; i < EntityArray.size(); ++i )
    {
        if ( EntityArray [ i ] == aEntity )
        {
            EntityArray.erase( EntityArray.begin() + i );
        }
    }

    delete aEntity;
    aEntity = nullptr;
}

void Scene::UnloadAllEntities()
{
    // Delete each entity that has been added
    for ( auto it = EntityArray.begin(); it != EntityArray.end(); ++it )
    {
        if ( *it != nullptr )
            delete ( *it );
    }

    EntityArray.clear();
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