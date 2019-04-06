#include "pch.h"
#include "Bullet.h"

COMPONENT_INIT( Bullet )


Bullet::Bullet()
{
    LOG_TRACE( "Create bullet!" );
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
}

void Bullet::Update(float deltaTime)
{
	// Move the position of this bullet in the forward direction
	
}

void Bullet::SaveComponentData( nlohmann::json & aCompData )
{
}
