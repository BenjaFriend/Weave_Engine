#include "Entity.h"
#include "Mesh.h"
#include "Material.h"
#include "SimpleShader.h"

struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;

using namespace DirectX;

Entity::Entity( Mesh* aMesh, Material* aMat )
    : EntityMesh( aMesh ), EntityMaterial( aMat )
{
    // Set default values for position, scale and rotation
    Position = DirectX::XMFLOAT3( 0.f, 0.f, 0.f );
    Scale = DirectX::XMFLOAT3( 1.f, 1.f, 1.f );
    Rotation = DirectX::XMFLOAT4();
}

// virtual destructor
Entity::~Entity()
{
    EntityMesh = nullptr;
    EntityMaterial = nullptr;
}

void Entity::MoveRelative( const float aX, const float aY, const float aZ )
{
    // rotate desired movement by our rotatoin
    XMVECTOR dir = XMVector3Rotate(
        XMVectorSet( aX, aY, aZ, 0 ),
        XMLoadFloat4( &Rotation ) );

    // Add to postion and store
    XMStoreFloat3(
        &Position,
        XMLoadFloat3( &Position ) + dir );
}

void Entity::MoveAbsolute( const float aX, const float aY, const float aZ )
{
    Position.x += aX;
    Position.y += aY;
    Position.z += aZ;
}

void Entity::PrepareMaterial( const DirectX::XMFLOAT4X4 & aView, const DirectX::XMFLOAT4X4 & aProjection )
{
    SimpleVertexShader* VertShader = EntityMaterial->GetVertexShader();
    SimplePixelShader* PixelShader = EntityMaterial->GetPixelShader();

    VertShader->SetMatrix4x4( "world", GetWorldMatrix() );
    VertShader->SetMatrix4x4( "view", aView );
    VertShader->SetMatrix4x4( "projection", aProjection );

    VertShader->SetShader();
    VertShader->CopyAllBufferData();

    PixelShader->SetShaderResourceView( "DiffuseTexture", EntityMaterial->GetDiffuseSRV() );
    PixelShader->SetShaderResourceView( "NormalTexture", EntityMaterial->GetNormalSRV() );
    PixelShader->SetSamplerState( "BasicSampler", EntityMaterial->GetSamplerState() );

    PixelShader->SetShader();
    PixelShader->CopyAllBufferData();
}


////////////////////////////////////////////////////
// Accessors
////////////////////////////////////////////////////

Mesh * Entity::GetEntityMesh() const
{
    return EntityMesh;
}

const Material* Entity::GetMaterial() const
{
    return EntityMaterial;
}

const DirectX::XMFLOAT3 & Entity::GetPosition() const
{
    // TODO: insert return statement here
    return Position;
}

void Entity::SetPosition( const DirectX::XMFLOAT3 & aNewPos )
{
    Position = aNewPos;
}

void Entity::SetPosition( const float aX, const float aY, const float aZ )
{
    Position.x = aX;
    Position.y = aY;
    Position.z = aZ;
}

const DirectX::XMFLOAT3 & Entity::GetScale() const
{
    return Scale;
}

void Entity::SetScale( const DirectX::XMFLOAT3 & aNewScale )
{
    Scale = aNewScale;
}

void Entity::SetScale( const float aX, const float aY, const float aZ )
{
    Scale.x = aX;
    Scale.y = aY;
    Scale.z = aZ;
}

const DirectX::XMFLOAT4 & Entity::GetRotation() const
{
    return Rotation;
}

void Entity::SetRotation( const DirectX::XMFLOAT4 & aNewRot )
{
    Rotation = aNewRot;
}

XMFLOAT4X4 Entity::GetWorldMatrix()
{
    XMMATRIX ScaleMatrix = XMMatrixScaling(
        Scale.x,
        Scale.y,
        Scale.z );

    XMMATRIX Rot = XMMatrixRotationRollPitchYaw(
        Rotation.x,
        Rotation.y,
        Rotation.z );

    XMMATRIX Pos = XMMatrixTranslation(
        Position.x,
        Position.y,
        Position.z );

    // Calculate the world matrix
    XMMATRIX WorldMM = ScaleMatrix * Rot * Pos;

    XMFLOAT4X4 World4x4;
    XMStoreFloat4x4( &World4x4, XMMatrixTranspose( WorldMM ) );	// Don't forget to transpose!

    return World4x4;
}

void Entity::SetIsActive( const bool aStatus )
{
    IsActive = aStatus;
}

const bool Entity::GetIsActive() const
{
    return IsActive;
}

