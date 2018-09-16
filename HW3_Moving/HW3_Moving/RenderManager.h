#pragma once

#include <vector>
#include "d3d11.h"	// ID3D11Buffer

/////////////////////////////////////////////////
// Forward Declarations
class Entity;
class Mesh;
struct Vertex;

/**
* Controls the rendering of meshes and entites 
* 
* @author Ben Hoffman
*/
class RenderManager
{
public:
	/** Default constructor for the render manager */
	RenderManager();

	/** Clean up entities and meshes */
	~RenderManager();

	/** 
	* Update all current entities
	* @param	DeltaTime
	*/
	void Update(float DeltaTime);

	/** Draw all entities in the scene */
	void DrawEntites();

	/** 
	* Add an entity to our rendermanager
	* 
	* @param EntityMesh		The mesh to use for this entity
	*/
	void AddEntity(Mesh* EntityMesh);

	/**
	* @brief				Creates a mesh and add it to our mesh vector
	* 
	* @param aDevice		The DX11 device to create the buffers
	* @param aVerts			Vertcies to use for this mesh
	* @param aNumVerts		Number of verticies this mesh has
	* @param aIndecies		The index locations of this mesh
	* @param aIndexCount	Number of indecies this msh has
	*/
	void AddMesh(ID3D11Device* aDevice, Vertex* aVerts, UINT aNumVerts, UINT* aIndecies, UINT aIndexCount);



private:

	/** Creation of meshes here */
	void InitalizeMeshes();

	/** Creation of entites here */
	void InitalizeEntities();

	/** The current entity count */
	size_t EntityCount	= 0;

	/** The current Mesh count */
	size_t MeshCount	= 0;

	/** Vector of meshes so we can easily keep track of them */
	std::vector<Mesh*> Meshes;

	/** Collection of all entities */
	std::vector<Entity*> Entities;

	
};

