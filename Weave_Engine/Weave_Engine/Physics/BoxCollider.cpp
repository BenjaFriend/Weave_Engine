#include "../stdafx.h"
#include "BoxCollider.h"
#include "../Entity/EntityManager.h"

using namespace Physics;

BoxCollider::BoxCollider( const glm::vec3 & aExtents )
    : Extents( aExtents )
{
    CenterOffset = glm::vec3( 0.f, 0.f, 0.f );
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

void BoxCollider::SaveObject( nlohmann::json & aOutFile )
{
}

const bool BoxCollider::Collides( const BoxCollider & aOther )
{
    glm::vec3 ownerPos = EntityManager::GetInstance()->GetEntity( owner )->GetTransform()->GetPosition();
    glm::vec3 otherPos = EntityManager::GetInstance()->GetEntity( aOther.GetOwner() )->GetTransform()->GetPosition();

    // Account for the offset of the collider
    ownerPos.x += this->CenterOffset.x;
    ownerPos.y += this->CenterOffset.y;
    ownerPos.z += this->CenterOffset.z;

    otherPos.x += aOther.CenterOffset.x;
    otherPos.y += aOther.CenterOffset.y;
    otherPos.z += aOther.CenterOffset.z;

    /*
      return (a.minX <= b.maxX && a.maxX >= b.minX) &&
             (a.minY <= b.maxY && a.maxY >= b.minY) &&
             (a.minZ <= b.maxZ && a.maxZ >= b.minZ);

    */
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
    glm::vec3 worldPos = EntityManager::GetInstance()->GetEntity( owner )->GetTransform()->GetPosition();
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
