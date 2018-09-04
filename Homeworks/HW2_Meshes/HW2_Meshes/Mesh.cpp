#include "Mesh.h"
#include "Vertex.h"

using namespace DirectX;

Mesh::Mesh(ID3D11Device* aDevice, Vertex* aVerts, UINT aNumVerts, UINT* aIndecies, UINT aIndexCount)
{
	// Create the VERTEX BUFFER description -----------------------------------
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * aNumVerts;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Create the proper struct to hold the initial vertex data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = aVerts;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	aDevice->CreateBuffer(&vbd, &initialVertexData, &VertexBuffer);

	// Create the INDEX BUFFER description ------------------------------------
	IndexCount = aIndexCount;

	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * IndexCount;         // 3 = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Create the proper struct to hold the initial index data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = aIndecies;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	aDevice->CreateBuffer(&ibd, &initialIndexData, &IndexBuffer);
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Mesh::~Mesh()
{
	// Release the buffers
	if (VertexBuffer)	{ VertexBuffer->Release(); }
	if (IndexBuffer)	{ IndexBuffer->Release();  }
}

ID3D11Buffer * const Mesh::GetVertexBuffer() const
{
	return VertexBuffer;
}

ID3D11Buffer * const Mesh::GetIndexBuffer() const
{
	return IndexBuffer;
}

const UINT Mesh::GetIndexCount() const
{
	return IndexCount;
}
