#pragma once

#include "../ECS/Component.h"
#include "PhysicsManager.h"

namespace Physics
{
    enum EPhysicsLayer
    {
        MOVEABLE,
        STATIC
    };

    class RigidBody : public ECS::Component<RigidBody>
    {
        RigidBody();

        ~RigidBody();

        virtual void DrawEditorGUI() override;

        virtual void SaveObject( nlohmann::json & aOutFile ) override;

        virtual const char* ComponentName() { return "RigidBody"; }
    };

}   // namespace Physics