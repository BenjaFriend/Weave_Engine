
#include "Bullet.h"
#include "Entity/Transform.h"
#include "Entity/IEntity.h"

COMPONENT_INIT( Bullet )


Bullet::Bullet()
{
    LOG_TRACE( "Create bullet!" );
}

Bullet::Bullet( float aSpeed )
    : Speed( aSpeed )
{

}

Bullet::Bullet( nlohmann::json const & aInitData )
{

}

Bullet::~Bullet()
{
    LOG_TRACE( "\tBullet dtor!" );
}

void Bullet::DrawEditorGUI()
{
#ifndef WEAVE_SERVER
    ImGui::DragFloat( "Speed", &Speed );
#endif
}

void Bullet::Update( float deltaTime )
{
    // Move the position of this bullet in the forward direction
    const glm::vec3 & forward = this->OwningEntity->GetTransform()->GetForward();
    glm::vec3 newPos = this->OwningEntity->GetTransform()->GetPosition();

    newPos += ( forward * deltaTime * Speed );
    
    // Set new position
    this->OwningEntity->GetTransform()->SetPosition( newPos );
}

void Bullet::SaveComponentData( nlohmann::json & aCompData )
{
}
