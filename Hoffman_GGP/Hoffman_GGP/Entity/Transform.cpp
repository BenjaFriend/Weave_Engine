#include "../stdafx.h"

#include "Transform.h"

Transform::Transform()
{

}

Transform::~Transform()
{

}


const VEC3 & Transform::GetPosition() const
{
    return Position;
}

void Transform::SetPosition( const VEC3 & aNewPos )
{
    Position = aNewPos;
}

void Transform::SetPosition( const float aX, const float aY, const float aZ )
{
    Position.x = aX;
    Position.y = aY;
    Position.z = aZ;
}

const VEC3 & Transform::GetScale() const
{
    return Scale;
}

void Transform::SetScale( const VEC3 & aNewScale )
{
    Scale = aNewScale;
}

void Transform::SetScale( const float aX, const float aY, const float aZ )
{
    Scale.x = aX;
    Scale.y = aY;
    Scale.z = aZ;
}

const DirectX::XMFLOAT4 & Transform::GetRotation() const
{
    return Rotation;
}

void Transform::SetRotation( const VEC4 & aNewRot )
{
    Rotation = aNewRot;
}

const DirectX::XMFLOAT4X4 Transform::GetWorldMatrix() const
{
    DirectX::XMMATRIX ScaleMatrix = DirectX::XMMatrixScaling(
        Scale.x,
        Scale.y,
        Scale.z );

    DirectX::XMMATRIX Rot = DirectX::XMMatrixRotationRollPitchYaw(
        Rotation.x,
        Rotation.y,
        Rotation.z );

    DirectX::XMMATRIX Pos = DirectX::XMMatrixTranslation(
        Position.x,
        Position.y,
        Position.z );

    // Calculate the world matrix
    DirectX::XMMATRIX WorldMM = ScaleMatrix * Rot * Pos;

    DirectX::XMFLOAT4X4 World4x4;
    DirectX::XMStoreFloat4x4( &World4x4, DirectX::XMMatrixTranspose( WorldMM ) );	// Don't forget to transpose!

    return World4x4;
}

