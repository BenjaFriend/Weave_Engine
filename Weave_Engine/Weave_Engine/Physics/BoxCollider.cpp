#include "../stdafx.h"
#include "BoxCollider.h"
#include "../Entity/Entity.h"

#define EXTENTS_SAVE_KEY            "Extents"
#define CENTER_OFFSET_SAVE_KEY      "CenterOffset"
#define IS_TRIGGER_SAVE_KEY         "IsTrigger"

COMPONENT_INIT( Physics::BoxCollider )

using namespace Physics;

BoxCollider::BoxCollider( const glm::vec3 & aExtents )
    : Extents( aExtents )
{
    CenterOffset = glm::vec3( 0.f, 0.f, 0.f );
    PhysicsManager::GetInstance()->AddBoxCollider( this );
}

Physics::BoxCollider::BoxCollider( nlohmann::json const & aInitData )
{
    Extents.x = aInitData [ EXTENTS_SAVE_KEY ] [ "X" ];
    Extents.y = aInitData [ EXTENTS_SAVE_KEY ] [ "Y" ];
    Extents.z = aInitData [ EXTENTS_SAVE_KEY ] [ "Z" ];

    CenterOffset.x = aInitData [ CENTER_OFFSET_SAVE_KEY ] [ "X" ];
    CenterOffset.y = aInitData [ CENTER_OFFSET_SAVE_KEY ] [ "Y" ];
    CenterOffset.z = aInitData [ CENTER_OFFSET_SAVE_KEY ] [ "Z" ];
    IsTrigger = aInitData [ IS_TRIGGER_SAVE_KEY ];
    PhysicsManager::GetInstance()->AddBoxCollider( this );
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::DrawEditorGUI()
{
    ImGui::InputFloat3( "Center Offset", ( float* ) &CenterOffset );
    ImGui::InputFloat3( "Extents", ( float* ) &Extents );
}

void BoxCollider::SaveComponentData( nlohmann::json & comp_data )
{
    comp_data [ EXTENTS_SAVE_KEY ] [ "X" ] = Extents.x;
    comp_data [ EXTENTS_SAVE_KEY ] [ "Y" ] = Extents.y;
    comp_data [ EXTENTS_SAVE_KEY ] [ "Z" ] = Extents.z;

    comp_data [ CENTER_OFFSET_SAVE_KEY ] [ "X" ] = CenterOffset.x;
    comp_data [ CENTER_OFFSET_SAVE_KEY ] [ "Y" ] = CenterOffset.y;
    comp_data [ CENTER_OFFSET_SAVE_KEY ] [ "Z" ] = CenterOffset.z;
    comp_data [ IS_TRIGGER_SAVE_KEY ] = IsTrigger;
}

const bool BoxCollider::Collides( const BoxCollider & aOther )
{
    SceneManagement::Scene* scene = SceneManagement::SceneManager::GetInstance()->GetActiveScene();
    Entity* owningEnt = scene->GetEntity( owner );
    Entity* otherEnt = scene->GetEntity( aOther.GetOwner() );

    glm::vec3 ownerPos = owningEnt->GetTransform()->GetPosition();
    glm::vec3 otherPos = otherEnt->GetTransform()->GetPosition();

    // Account for the offset of the collider
    ownerPos.x += this->CenterOffset.x;
    ownerPos.y += this->CenterOffset.y;
    ownerPos.z += this->CenterOffset.z;

    otherPos.x += aOther.CenterOffset.x;
    otherPos.y += aOther.CenterOffset.y;
    otherPos.z += aOther.CenterOffset.z;

    return
        ( ownerPos.x - Extents.x / 2 <= otherPos.x + aOther.Extents.x / 2 && ownerPos.x + Extents.x / 2 >= otherPos.x - aOther.Extents.x / 2 ) &&
        ( ownerPos.y - Extents.y / 2 <= otherPos.y + aOther.Extents.y / 2 && ownerPos.y + Extents.y / 2 >= otherPos.y - aOther.Extents.y / 2 ) &&
        ( ownerPos.z - Extents.z / 2 <= otherPos.z + aOther.Extents.z / 2 && ownerPos.z + Extents.z / 2 >= otherPos.z - aOther.Extents.z / 2 );
}

void BoxCollider::SetCenterOffset( const glm::vec3 & aVal )
{
    CenterOffset = aVal;
}

const glm::vec3 & BoxCollider::GetCenterOffset() const
{
    return CenterOffset;
}

const glm::vec3 Physics::BoxCollider::GetPosition() const
{
    SceneManagement::Scene* scene = SceneManagement::SceneManager::GetInstance()->GetActiveScene();
    Entity* ent = scene->GetEntity( owner );

    glm::vec3 worldPos = ent->GetTransform()->GetPosition();

    worldPos.x += CenterOffset.x;
    worldPos.y += CenterOffset.y;
    worldPos.z += CenterOffset.z;
    return worldPos;
}

void BoxCollider::SetExtents( const glm::vec3 & aVal )
{
    Extents = aVal;
}

const glm::vec3 & BoxCollider::GetExtents() const
{
    return Extents;
}

const bool BoxCollider::SetIsTrigger() const
{
    return IsTrigger;
}

void BoxCollider::SetIsTrigger( bool aVal )
{
    IsTrigger = aVal;
}