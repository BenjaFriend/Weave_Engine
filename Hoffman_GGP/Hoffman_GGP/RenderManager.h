#pragma once
#include "DebugSettings.h"

#include <vector>
#include "d3d11.h"	// ID3D11Buffer

/////////////////////////////////////////////////
// Forward Declarations
class Entity;
class Mesh;
struct Vertex;

/// <summary>
/// Controls the rendering of meshes and entities 
/// </summary>
/// <author>Ben Hoffman</author>
class RenderManager
{
public:
	/** Default constructor for the render manager */
	RenderManager();

	/** Clean up entities and meshes */
	~RenderManager();

    /// <summary>
    /// all current entities
    /// </summary>
    /// <param name="DeltaTime">Time between frames</param>
	void Update(const float DeltaTime);

	/** Draw all entities in the scene */
	void DrawEntites();

    /// <summary>
    /// Add an entity to our render manager
    /// </summary>
    /// <param name="EntityMesh">The mesh to use for this entity</param>
	void AddEntity(Mesh* EntityMesh);

    /// <summary>
    /// Creates a mesh and add it to our mesh vector
    /// </summary>
    /// <param name="aDevice">The DX11 device to create the buffers</param>
    /// <param name="aVerts">Vertcies to use for this mesh</param>
    /// <param name="aNumVerts">Number of verticies this mesh has</param>
    /// <param name="aIndecies">The index locations of this mesh</param>
    /// <param name="aIndexCount">Number of indecies this msh has</param>
	void AddMesh(ID3D11Device* aDevice, Vertex* aVerts, UINT aNumVerts, UINT* aIndecies, UINT aIndexCount);


private:

	/** Creation of meshes here */
	void InitalizeMeshes();

	/** Creation of entities here */
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

