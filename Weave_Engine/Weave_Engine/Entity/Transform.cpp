#include "../stdafx.h"

#include "Transform.h"

#define POS_SAVE_KEY        "Pos"
#define SCALE_SAVE_KEY      "Scale"
#define ROT_SAVE_KEY        "Rot"

Transform::Transform()
{
    Position = glm::vec3( 0.f );
    Scale = glm::vec3( 1.f );
    Rotation = glm::vec3( 0.f );
}

Transform::~Transform()
{

}

void Transform::DrawEditorGUI()
{
    if ( ImGui::CollapsingHeader( "Transform" ) )
    {
        ImGui::InputFloat3( "Position", ( float* ) &Position );

        ImGui::InputFloat3( "Scale", ( float* ) &Scale );

        ImGui::InputFloat3( "Rotation", ( float* ) &Rotation );
    }
}

void Transform::SaveObject( nlohmann::json & aOutFile )
{
    nlohmann::json comp_data = nlohmann::json::object();
    comp_data [ COMP_SAVE_KEY ] = ComponentName();

    comp_data [ POS_SAVE_KEY ] [ "X" ] = Position.x;
    comp_data [ POS_SAVE_KEY ] [ "Y" ] = Position.y;
    comp_data [ POS_SAVE_KEY ] [ "Z" ] = Position.z;

    comp_data [ SCALE_SAVE_KEY ] [ "X" ] = Scale.x;
    comp_data [ SCALE_SAVE_KEY ] [ "Y" ] = Scale.y;
    comp_data [ SCALE_SAVE_KEY ] [ "Z" ] = Scale.z;

    comp_data [ ROT_SAVE_KEY ] [ "X" ] = Rotation.x;
    comp_data [ ROT_SAVE_KEY ] [ "Y" ] = Rotation.y;
    comp_data [ ROT_SAVE_KEY ] [ "Z" ] = Rotation.z;

    if ( aOutFile.is_array() )
    {
        aOutFile.push_back( comp_data );
    }
}

void Transform::MoveRelative( const float aX, const float aY, const float aZ )
{
    // #TODO Make this actually take into account the rotation
    Position.x += aX;
    Position.y += aY;
    Position.z += aZ;
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

const glm::vec3 & Transform::GetScale() const
{
    return Scale;
}

void Transform::SetScale( const glm::vec3 & aNewScale )
{
    Scale = aNewScale;
}

void Transform::SetScale( const float aX, const float aY, const float aZ )
{
    Scale.x = aX;
    Scale.y = aY;
    Scale.z = aZ;
}

const glm::vec3 & Transform::GetRotation() const
{
    return Rotation;
}

void Transform::SetRotation( const glm::vec3 & aNewRot )
{
    Rotation = aNewRot;
}

const glm::highp_mat4 Transform::GetWorldMatrix() const
{
    // World = Scale * rot * pos
    glm::mat4 worldMat = glm::identity<glm::mat4>();
    worldMat = glm::translate( worldMat, Position );
    worldMat = worldMat * glm::yawPitchRoll( Rotation.y, Rotation.x, Rotation.z );
    worldMat = glm::scale( worldMat, Scale );

    // We need to transpose the world matrix because of the differences 
    // between GLM and DX11
    return glm::transpose( worldMat );
}