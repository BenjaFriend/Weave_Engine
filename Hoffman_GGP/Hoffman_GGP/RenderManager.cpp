#include "RenderManager.h"
#include "Entity.h"
#include "Mesh.h"

RenderManager::RenderManager()
{
	InitalizeMeshes();
	InitalizeEntities();
}

RenderManager::~RenderManager()
{
	for (size_t i = 0; i < EntityCount; ++i)
	{
		Entity* CurrentEntity = Entities[i];
		if(CurrentEntity) delete CurrentEntity;
	}

	Entities.clear();
	EntityCount = 0;

	for (size_t i = 0; i < MeshCount; ++i)
	{
		Mesh* CurrentMesh = Meshes[i];
		if (CurrentMesh) delete CurrentMesh;
	}

	Meshes.clear();
	MeshCount = 0;
}

void RenderManager::InitalizeMeshes()
{

}

void RenderManager::InitalizeEntities()
{

}

void RenderManager::AddEntity(Mesh * EntityMesh)
{
	if (EntityMesh)
	{
		Entities.push_back(new Entity(EntityMesh));
		++EntityCount;
	}
}

void RenderManager::AddMesh(ID3D11Device * aDevice, Vertex * aVerts, UINT aNumVerts, UINT * aIndecies, UINT aIndexCount)
{
	Mesh* CurrentMesh = new Mesh(aDevice, aVerts, aNumVerts, aIndecies, aIndexCount);
	Meshes.push_back(CurrentMesh);
	++MeshCount;
}

void RenderManager::Update(float DeltaTime)
{

}

void RenderManager::DrawEntites()
{

}