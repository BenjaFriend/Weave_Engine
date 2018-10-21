#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"

// Singleton definitions
ResourceManager* ResourceManager::Instance = nullptr;

ResourceManager * ResourceManager::Initalize( ID3D11Device* aDevice )
{
    assert( Instance == nullptr && aDevice != nullptr );

    Instance = new ResourceManager( aDevice );

    return Instance;
}

ResourceManager * ResourceManager::GetInstance()
{
    assert( Instance != nullptr );

    return Instance;
}

void ResourceManager::ReleaseInstance()
{
    if ( Instance != nullptr )
    {
        delete Instance;
        Instance = nullptr;
    }
}

const UINT ResourceManager::LoadMesh( char * aFileName )
{
    Meshes.push_back( new Mesh( currentDevice, aFileName ) );
    return Meshes.size() - 1;
}

// Private constructor
ResourceManager::ResourceManager( ID3D11Device* aDevice )
    : currentDevice( aDevice )
{

}

// Private destructor
ResourceManager::~ResourceManager()
{
    UnloadMeshes();

    UnloadMaterials();

    // Unload shaders

    currentDevice = nullptr;
}

void ResourceManager::UnloadMeshes()
{
    // Delete each entity that has been added
    for ( auto it = Meshes.begin(); it != Meshes.end(); ++it )
    {
        delete ( *it );
    }

    Meshes.clear();
}

void ResourceManager::UnloadMaterials()
{
    // Delete each entity that has been added
    for ( auto it = Materials.begin(); it != Materials.end(); ++it )
    {
        delete ( *it );
    }

    Materials.clear();
}
