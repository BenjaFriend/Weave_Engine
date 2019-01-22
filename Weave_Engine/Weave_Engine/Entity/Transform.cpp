#include "../stdafx.h"

#include "Transform.h"

Transform::Transform()
{
    Position = glm::vec3( 0.f, 0.f, 0.f );
    Scale = DirectX::XMFLOAT3( 1.f, 1.f, 1.f );
    Rotation = DirectX::XMFLOAT4();
}

Transform::~Transform()
{

}

void Transform::DrawEditorGUI()
{
    if ( ImGui::CollapsingHeader( "Transform" ) )
    {
        ImGui::InputFloat3( "Position", ( float* ) &Position);

        ImGui::InputFloat3( "Scale", ( float* ) &Scale );

        ImGui::InputFloat4( "Rotation", ( float* ) &Rotation);
    }
}

void Transform::SaveObject( nlohmann::json & aOutFile )
{
    // #TODO Save the pos, rot and scale of this object 
    aOutFile [ "unimplemented" ] = 0;
}

void Transform::MoveRelative( const float aX, const float aY, const float aZ )
{
    //using namespace DirectX;
    // rotate desired movement by our rotation
    //XMVECTOR dir = XMVector3Rotate(
    //    XMVectorSet( aX, aY, aZ, 0 ),
    //    XMLoadFloat4( &Rotation ) );

    // Add to position and store
    //XMStoreFloat3(
    //    &Position,
    //    XMLoadFloat3( &Position ) + dir );
    //

    Position.x += aX;
    
}

void Transform::MoveAbsolute( const float aX, const float aY, const float aZ )
{
    Position.x += aX;
    Position.y += aY;
    Position.z += aZ;
}

const glm::vec3 & Transform::GetPosition() const
{
    return Position;
}

void Transform::SetPosition( const glm::vec3 & aNewPos )
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

const VEC4 & Transform::GetRotation() const
{
    return Rotation;
}

void Transform::SetRotation( const VEC4 & aNewRot )
{
    Rotation = aNewRot;
}

const VEC4x4 Transform::GetWorldMatrix() const
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