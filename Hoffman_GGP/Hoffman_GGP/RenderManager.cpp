#include "RenderManager.h"
#include "Entity.h"
#include "Mesh.h"

// Singleton requirement
RenderManager* RenderManager::Instance = nullptr;

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
    ++MeshCount;
}

void RenderManager::DrawEntites()
{

}