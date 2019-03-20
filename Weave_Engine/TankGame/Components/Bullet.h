#pragma once

#include <ECS/Component.h>


class Bullet : public ECS::Component<Bullet>
{
public:

    COMPONENT( Bullet );

    Bullet();

    Bullet( nlohmann::json const & aInitData );

    ~Bullet();

    virtual void DrawEditorGUI() override;

protected:

    virtual void SaveComponentData( nlohmann::json & aCompData ) override;

};