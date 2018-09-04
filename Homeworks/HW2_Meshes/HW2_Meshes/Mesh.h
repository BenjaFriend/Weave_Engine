#pragma once

#include "d3d11.h"

/**
* Generic class for handling the vertex and index buffers of meshes
*/
class Mesh
{
public:
	
	Mesh(ID3D11Device* aDevice, struct Vertex* aVerts, UINT aNumVerts, UINT* aIndecies, UINT aIndexCount);
	
	~Mesh();

	/** Pointer to this objects Vertex buffer */
	ID3D11Buffer* const GetVertexBuffer() const;
	/** Pointer to this object's index buffer */
	ID3D11Buffer* const GetIndexBuffer() const;
	/** Number of indecies are in the index buffer */
	const UINT GetIndexCount() const;


private:
	
	/** The vertex buffer for this mesh class */
	ID3D11Buffer* VertexBuffer = nullptr;

	/**
	* The index buffer -- Integers which specify the order
	* the GPU should use the verticies
	*/
	ID3D11Buffer* IndexBuffer = nullptr;

	/** Specifies how many indicies are in the index buffer */
	UINT IndexCount;

};

