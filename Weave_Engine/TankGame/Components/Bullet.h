#pragma once

#include <ECS/Component.h>


class Bullet : public ECS::Component<Bullet>
{
public:

    COMPONENT( Bullet );

    Bullet();

    Bullet( float aSpeed, float aLifetime );

    Bullet( nlohmann::json const & aInitData );

    ~Bullet();

    virtual void DrawEditorGUI() override;

    virtual void Update( float deltaTime ) override;

protected:

    virtual void SaveComponentData( nlohmann::json & aCompData ) override;

private:

    /** The speed at which this bullet will move forward */
    float Speed = 10.0f;

    /** The lifetime of this bullet is how long it will be active in the world before reseting */
    const float Lifetime = 4.0f;

    float TimeSinceSpawn = 0.0f;

};