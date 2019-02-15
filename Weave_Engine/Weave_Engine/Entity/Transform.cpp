#include "../stdafx.h"

#include "Transform.h"

#define POS_SAVE_KEY        "Pos"
#define SCALE_SAVE_KEY      "Scale"
#define ROT_SAVE_KEY        "Rot"

static ECS::IComponent::ConcreteFactory< Transform > CompFactory = {};

Transform::Transform()
{
    Position = glm::vec3( 0.f );
    Scale = glm::vec3( 1.f );
    Rotation = glm::vec3( 0.f );
}

Transform::Transform( nlohmann::json const & aInitData )
{
    // Maye this works? 
    Position.x = aInitData [ POS_SAVE_KEY ] [ "X" ];
    Position.y = aInitData [ POS_SAVE_KEY ] [ "Y" ];
    Position.z = aInitData [ POS_SAVE_KEY ] [ "Z" ];

    Scale.x = aInitData [ SCALE_SAVE_KEY ] [ "X" ];
    Scale.y = aInitData [ SCALE_SAVE_KEY ] [ "Y" ];
    Scale.z = aInitData [ SCALE_SAVE_KEY ] [ "Z" ];

    Rotation.x = aInitData [ ROT_SAVE_KEY ] [ "X" ];
    Rotation.y = aInitData [ ROT_SAVE_KEY ] [ "Y" ];
    Rotation.z = aInitData [ ROT_SAVE_KEY ] [ "Z" ];
}

Transform::~Transform()
{

}

void Transform::DrawEditorGUI()
{
    ImGui::InputFloat3( "Position", ( float* ) &Position );
    ImGui::InputFloat3( "Scale", ( float* ) &Scale );
    ImGui::InputFloat3( "Rotation", ( float* ) &Rotation );
}

void Transform::SaveComponentData( nlohmann::json & comp_data )
{
    comp_data [ POS_SAVE_KEY ] [ "X" ] = Position.x;
    comp_data [ POS_SAVE_KEY ] [ "Y" ] = Position.y;
    comp_data [ POS_SAVE_KEY ] [ "Z" ] = Position.z;

    comp_data [ SCALE_SAVE_KEY ] [ "X" ] = Scale.x;
    comp_data [ SCALE_SAVE_KEY ] [ "Y" ] = Scale.y;
    comp_data [ SCALE_SAVE_KEY ] [ "Z" ] = Scale.z;

    comp_data [ ROT_SAVE_KEY ] [ "X" ] = Rotation.x;
    comp_data [ ROT_SAVE_KEY ] [ "Y" ] = Rotation.y;
    comp_data [ ROT_SAVE_KEY ] [ "Z" ] = Rotation.z;
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

void Transform::SetPosition( const float aX, const float aY, const float aZ )
{
    Position.x = aX;
    Position.y = aY;
    Position.z = aZ;
}

inline void Transform::SetScale( const float aX, const float aY, const float aZ )
{
    Scale.x = aX;
    Scale.y = aY;
    Scale.z = aZ;
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