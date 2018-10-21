#include "RenderManager.h"
#include "../Entity/Entity.h"
#include "Mesh.h"

// Singleton requirement
RenderManager* RenderManager::Instance = nullptr;

RenderManager* RenderManager::GetInstance()
{
    if ( Instance == nullptr )
    {
        Instance = new RenderManager();
    }
    return Instance;
}

void RenderManager::ReleaseInstance()
{
    if ( Instance != nullptr )
    {
        delete Instance;
        Instance = nullptr;
    }
}

// Private constructor
RenderManager::RenderManager()
{

}

// Private destructor
RenderManager::~RenderManager()
{
    
}

void RenderManager::AddMesh( ID3D11Device * aDevice, Vertex * aVerts, UINT aNumVerts, UINT * aIndecies, UINT aIndexCount )
{
    Mesh* CurrentMesh = new Mesh( aDevice, aVerts, aNumVerts, aIndecies, aIndexCount );
    Meshes.push_back( CurrentMesh );
}