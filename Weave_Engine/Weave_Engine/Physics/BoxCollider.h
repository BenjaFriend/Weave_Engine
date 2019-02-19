#pragma once

#include "../stdafx.h"

#include "../ECS/Component.h"
#include "PhysicsManager.h"
#include "../Scenes/SceneManager.h"
#include "../Scenes/Scene.h"

namespace Physics
{
    /// <summary>
    /// Box collider component for basic collisions
    /// </summary>
    class BoxCollider : public ECS::Component<BoxCollider>
    {
    public:

        COMPONENT( BoxCollider );

        /// <summary>
        /// A box collider component to handle simple box collision 
        /// </summary>
        /// <param name="aExtents">The extents of this object</param>
        BoxCollider( const glm::vec3 & aExtents = glm::vec3( 1.f, 1.f, 1.f ) );

        BoxCollider( nlohmann::json const & aInitData );

        ~BoxCollider();

        virtual void DrawEditorGUI() override;

        const bool Collides( const BoxCollider & aOther );

        ////////////////////////////////////////////////////
        // Accessors
        ////////////////////////////////////////////////////

        void SetCenterOffset( const glm::vec3 & aVal );

        const glm::vec3 & GetCenterOffset() const;

        /// <summary>
        /// Get the world position of this box collider with respect 
        /// to the owning object.
        /// </summary>
        /// <returns>VEC3 that is the center of this collider</returns>
        const glm::vec3 GetPosition() const;

        void SetExtents( const glm::vec3 & aVal );

        const glm::vec3 & GetExtents() const;

        const bool SetIsTrigger() const;

        void SetIsTrigger( bool aVal );

    protected:

        virtual void SaveComponentData( nlohmann::json & aOutFile ) override;

    private:

        glm::vec3 CenterOffset;

        glm::vec3 Extents;

        bool IsTrigger = false;

    };
}   // namespace Physics