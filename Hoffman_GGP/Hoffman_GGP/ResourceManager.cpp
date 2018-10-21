#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"

// Singleton definitions
ResourceManager* ResourceManager::Instance = nullptr;


ResourceManager * ResourceManager::Initalize()
{
    return nullptr;
}

ResourceManager * ResourceManager::GetInstance()
{
    return nullptr;
}

void ResourceManager::ReleaseInstance()
{
}

Mesh * ResourceManager::LoadMesh( const char * aFileName )
{
    return nullptr;
}

// Private constructor
ResourceManager::ResourceManager()
{
}

// Private destructor
ResourceManager::~ResourceManager()
{


}

void ResourceManager::UnloadMeshes()
{
}

void ResourceManager::UnloadMaterials()
{
}
