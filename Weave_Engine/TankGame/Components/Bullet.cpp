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

void Bullet::SaveComponentData( nlohmann::json & aCompData )
{
}
