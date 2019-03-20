#pragma once

#include <vector>
#include "d3d11.h"	// ID3D11Buffer

/////////////////////////////////////////////////
// Forward Declarations
class Mesh;
struct Vertex;

/// <summary>
/// In charge of the creation of any SRV's, materials, or meshes
/// that need to be loaded in.
/// </summary>
/// <author>Ben Hoffman</author>
class RenderManager
{
public:

    /// <summary>
    /// Get an instance of the render manager, 
    /// </summary>
    /// <returns>Static instance of a render manager</returns>
    static RenderManager* GetInstance();

    /// <summary>
    /// Release the current instance of the render manager
    /// </summary>
    static void ReleaseInstance();

    /// <summary>
    /// Creates a mesh and add it to our mesh vector
    /// </summary>
    /// <param name="aDevice">The DX11 device to create the buffers</param>
    /// <param name="aVerts">Vertcies to use for this mesh</param>
    /// <param name="aNumVerts">Number of verticies this mesh has</param>
    /// <param name="aIndecies">The index locations of this mesh</param>
    /// <param name="aIndexCount">Number of indecies this msh has</param>
    void AddMesh( ID3D11Device* aDevice, Vertex* aVerts, UINT aNumVerts, UINT* aIndecies, UINT aIndexCount );

    // We don't want anything making copies of this class so delete these operators
    RenderManager( RenderManager const& ) = delete;
    void operator=( RenderManager const& ) = delete;

private:

    /** Default constructor for the render manager */
    RenderManager();

    /** Clean up entities and meshes */
    ~RenderManager();

    static RenderManager* Instance;

    /** Vector of meshes so we can easily keep track of them */
    std::vector<Mesh*> Meshes;

};