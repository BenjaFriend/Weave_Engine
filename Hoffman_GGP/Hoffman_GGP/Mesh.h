#pragma once

#include "d3d11.h"	// ID3D11Buffer

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
	Mesh(ID3D11Device* aDevice, Vertex* aVerts, UINT aNumVerts, UINT* aIndecies, UINT aIndexCount);
	
	/** Destructor to release DX11 objects */
	~Mesh();

	//////////////////////////////////////////////////////////////////
	// Accessors 
	//////////////////////////////////////////////////////////////////

	/** Pointer to this objects Vertex buffer */
	ID3D11Buffer* const GetVertexBuffer() const;
	/** Pointer to this object's index buffer */
	ID3D11Buffer* const GetIndexBuffer() const;
	/** Number of indicies are in the index buffer */
	const UINT GetIndexCount() const;


private:
	
	/** The vertex buffer for this mesh class */
	ID3D11Buffer* VertexBuffer = nullptr;

	/**
	* The index buffer -- Integers which specify the order
	* the GPU should use the vertices's
	*/
	ID3D11Buffer* IndexBuffer = nullptr;

	/** Specifies how many indicies are in the index buffer */
	UINT IndexCount;

};

