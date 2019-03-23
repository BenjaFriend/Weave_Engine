#include "stdafx.h"

#include "Mesh.h"
#include "Vertex.h"

Mesh::Mesh( ID3D11Device* aDevice, Vertex* aVerts, UINT aNumVerts, UINT* aIndecies, UINT aIndexCount )
{
    CreateBuffers( aDevice, aVerts, aNumVerts, aIndecies, aIndexCount );
}

Mesh::Mesh( ID3D11Device* aDevice, FileName objFile )
    : MeshFileName( objFile )
{ 
    std::string fileName( objFile.begin(), objFile.end() );

    unsigned int vertCounter = 0;        // Count of vertices/indices

    LOG_TRACE( "Load asset: {}", fileName );
    const aiScene* scene = aiImportFile( fileName.c_str(),
        aiProcess_ImproveCacheLocality  |
        aiProcess_SortByPType           |
        aiProcess_FixInfacingNormals    |
        aiProcess_GenSmoothNormals      |
        aiProcess_GenUVCoords
    );

    if ( !scene )
    {
        LOG_ERROR( "Could not import file: {}", aiGetErrorString() );
        return;
    }

    if ( scene->HasMeshes() )
    {
        for ( size_t i = 0; i < scene->mNumMeshes; ++i )
        {
            const aiMesh* curMesh = scene->mMeshes [ i ];
            assert( curMesh != nullptr );
            std::vector<Vertex> Verts;
            std::vector<UINT> Indices;

            const aiVector3D Zero3D( 0.0f, 0.0f, 0.0f );
            for ( size_t j = 0; j < curMesh->mNumVertices; ++j )
            {
                const aiVector3D* pPos = &( curMesh->mVertices [ j ] );
                const aiVector3D* pNormal = &( curMesh->mNormals [ j ] );
                const aiVector3D* pTexCoord = curMesh->HasTextureCoords( 0 ) ? &( curMesh->mTextureCoords [ 0 ] [ j ] ) : &Zero3D;
                Vertex v = {};
                // Get vert normals
                v.Normal.x = pNormal->x;
                v.Normal.y = pNormal->y;
                v.Normal.z = pNormal->z;
                // Get vert UV/texture coords
                v.UV.x = pTexCoord->x;
                v.UV.y = pTexCoord->y;
                // Get vert position 
                v.Position.x = pPos->x;
                v.Position.y = pPos->y;
                v.Position.z = pPos->z;
                Verts.push_back( v );
            }

            // Load in indicies
            for ( size_t j = 0; j < curMesh->mNumFaces; ++j )
            {
                const aiFace& Face = curMesh->mFaces [ i ];
                assert( Face.mNumIndices == 3 );
                Indices.push_back( vertCounter++ );
                Indices.push_back( vertCounter++ );
                Indices.push_back( vertCounter++ );
            }
            CreateBuffers( aDevice, &Verts [ 0 ], vertCounter, &Indices [ 0 ], vertCounter );
        }
    }

    aiReleaseImport( scene );
    LOG_TRACE( "Done import!" );
}

Mesh::~Mesh()
{
    // Release the buffers
    if ( VertexBuffer ) { VertexBuffer->Release(); }
    if ( IndexBuffer ) { IndexBuffer->Release(); }
}

void Mesh::CreateBuffers( ID3D11Device * aDevice, Vertex * aVerts, UINT aNumVerts, UINT * aIndecies, UINT aIndexCount )
{

    CalculateTangents( aVerts, aNumVerts, aIndecies, aIndexCount );

    // Create the VERTEX BUFFER description -----------------------------------
    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof( Vertex ) * aNumVerts;
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
    aDevice->CreateBuffer( &vbd, &initialVertexData, &VertexBuffer );

    // Create the INDEX BUFFER description ------------------------------------
    IndexCount = aIndexCount;

    // - The description is created on the stack because we only need
    //    it to create the buffer.  The description is then useless.
    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof( int ) * IndexCount;         // 3 = number of indices in the buffer
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
    aDevice->CreateBuffer( &ibd, &initialIndexData, &IndexBuffer );
}

// Calculates the tangents of the vertices in a mesh
// Code adapted from: http://www.terathon.com/code/tangent.html
void Mesh::CalculateTangents( Vertex* verts, int numVerts, unsigned int* indices, int numIndices )
{
    // Reset tangents
    for ( int i = 0; i < numVerts; i++ )
    {
        verts [ i ].Tangent = glm::vec3( 0, 0, 0 );
    }

    // Calculate tangents one whole triangle at a time
    for ( int i = 0; i < numVerts;)
    {
        // Grab indices and vertices of first triangle
        unsigned int i1 = indices [ i++ ];
        unsigned int i2 = indices [ i++ ];
        unsigned int i3 = indices [ i++ ];
        Vertex* v1 = &verts [ i1 ];
        Vertex* v2 = &verts [ i2 ];
        Vertex* v3 = &verts [ i3 ];

        // Calculate vectors relative to triangle positions
        float x1 = v2->Position.x - v1->Position.x;
        float y1 = v2->Position.y - v1->Position.y;
        float z1 = v2->Position.z - v1->Position.z;

        float x2 = v3->Position.x - v1->Position.x;
        float y2 = v3->Position.y - v1->Position.y;
        float z2 = v3->Position.z - v1->Position.z;

        // Do the same for vectors relative to triangle uv's
        float s1 = v2->UV.x - v1->UV.x;
        float t1 = v2->UV.y - v1->UV.y;

        float s2 = v3->UV.x - v1->UV.x;
        float t2 = v3->UV.y - v1->UV.y;

        // Create vectors for tangent calculation
        float r = 1.0f / ( s1 * t2 - s2 * t1 );

        float tx = ( t2 * x1 - t1 * x2 ) * r;
        float ty = ( t2 * y1 - t1 * y2 ) * r;
        float tz = ( t2 * z1 - t1 * z2 ) * r;

        // Adjust tangents of each vert of the triangle
        v1->Tangent.x += tx;
        v1->Tangent.y += ty;
        v1->Tangent.z += tz;

        v2->Tangent.x += tx;
        v2->Tangent.y += ty;
        v2->Tangent.z += tz;

        v3->Tangent.x += tx;
        v3->Tangent.y += ty;
        v3->Tangent.z += tz;
    }

    // Ensure all of the tangents are orthogonal to the normals
    for ( int i = 0; i < numVerts; i++ )
    {
        // Tangent = Normal ( T - N * Dot( N, T ) )
        glm::vec3 norm = verts [ i ].Normal;
        glm::vec3 tan = verts [ i ].Tangent;

        tan = glm::normalize( tan - norm * glm::dot( norm, tan ) );
        verts [ i ].Tangent = tan;
    }
}

//////////////////////////////////////////////////////////////////
// Accessors 
//////////////////////////////////////////////////////////////////

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

