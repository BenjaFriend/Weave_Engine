#pragma once

#include "d3d11.h"	// ID3D11Buffer
#include <vector>   // std::vector
#include <iostream> // std::ifstream
#include <fstream>  // std::ifstream

/////////////////////////////////////////////////
// Forward Declarations
struct Vertex;

/// <summary>
/// Generic class for handling the vertex and index buffers of meshes
/// </summary>
/// <author>Ben Hoffman</author>
class Mesh
{
public:

    /// <summary>
    /// Creates a mesh with the given vertecies and index parameters
    /// </summary>
    /// <param name="aDevice">The DX11 device to create the buffers</param>
    /// <param name="aVerts">Vertcies to use for this mesh</param>
    /// <param name="aNumVerts">Number of verticies this mesh has</param>
    /// <param name="aIndecies">The index locations of this mesh</param>
    /// <param name="aIndexCount">Number of indecies this msh has</param>
    Mesh( ID3D11Device* aDevice, Vertex* aVerts, UINT aNumVerts, UINT* aIndecies, UINT aIndexCount );

    /// <summary>
    /// Create a mesh based off of the mesh file
    /// </summary>
    /// <param name="aMeshFile">File name to load</param>
    Mesh( ID3D11Device* aDevice, FileName objFile );

    /** Destructor to release DX11 objects */
    ~Mesh();

    //////////////////////////////////////////////////////////////////
    // Accessors 
    //////////////////////////////////////////////////////////////////

    /** Pointer to this objects Vertex buffer */
    ID3D11Buffer* const GetVertexBuffer() const;
    /** Pointer to this object's index buffer */
    ID3D11Buffer* const GetIndexBuffer() const;
    /** Number of indices are in the index buffer */
    const UINT GetIndexCount() const;

    FORCE_INLINE const FileName GetMeshFileName() const { return MeshFileName; }

private:

    void Mesh::CalculateTangents( Vertex* verts, int numVerts, unsigned int* indices, int numIndices );

    /** The vertex buffer for this mesh class */
    ID3D11Buffer * VertexBuffer = nullptr;

    /**
    * The index buffer -- Integers which specify the order
    * the GPU should use the vertices's
    */
    ID3D11Buffer* IndexBuffer = nullptr;

    /** Specifies how many indices are in the index buffer */
    UINT IndexCount;

    /// <summary>
    /// Creates a mesh with the given vertices and index parameters
    /// </summary>
     /// <param name="aDevice">The DX11 device to create the buffers</param>
    /// <param name="aVerts">Vertcies to use for this mesh</param>
    /// <param name="aNumVerts">Number of verticies this mesh has</param>
    /// <param name="aIndecies">The index locations of this mesh</param>
    /// <param name="aIndexCount">Number of indecies this msh has</param>
    void CreateBuffers( ID3D11Device* aDevice, Vertex* aVerts, UINT aNumVerts, UINT* aIndecies, UINT aIndexCount );

    /** The file name of that was used to load this mesh */
    FileName MeshFileName;
};

