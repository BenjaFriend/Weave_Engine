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
    public:

        RigidBody( float aMass );

        ~RigidBody();

        virtual void DrawEditorGUI() override;

        virtual void SaveObject( nlohmann::json & aOutFile ) override;

        virtual const char* ComponentName() { return "RigidBody"; }

        /// <summary>
        /// Apply a force to this entity
        /// </summary>
        /// <param name="aForce">The force to apply</param>
        void ApplyForce( const VEC3 aForce );

        /// <summary>
        /// Apply the current acceleration to the velocity and set acceleration to 0
        /// </summary>
        void ApplyAcceleration();

        ////////////////////////////////////////////////////
        // Accessors
        ////////////////////////////////////////////////////

        void SetVelocity( const VEC3& aVel );

        const VEC3 GetVelocity() const;

        const float GetMass() const;

        void SetMass( const float aMass );

        const EPhysicsLayer GetPhysicsLayer() const;
    private:

        /** The mass of this object */
        float Mass = 1.0f;

        VEC3 Velocity;

        VEC3 Acceleration;

        EPhysicsLayer Layer;
    };

}   // namespace Physics