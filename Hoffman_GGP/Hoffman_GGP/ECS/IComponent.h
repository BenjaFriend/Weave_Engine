#pragma once
#include "../stdafx.h"

namespace ECS
{
    using ComponentID = size_t;
    using ComponentTypeId = size_t;
    using EntityID = size_t;

    class IComponent
    {
        friend class ComponentManager;

    public:

        IComponent() {}

        virtual ~IComponent() {}

        virtual void DrawEditorGUI()
        {

        }

        inline const bool operator==( const IComponent& other ) const { return id == other.id; }
        inline const bool operator!=( const IComponent& other ) const { return id == other.id; }

        /////////////////////////////////////////////////    
        // Accessors 

        inline const ComponentID GetComponentId() const { return this->id; }

        inline const bool IsEnabled() const { return this->isEnabled; }

        inline void SetSenabled( bool aEnabledState ) { this->isEnabled = aEnabledState; }

        const EntityID& GetOwner() const { return this->owner; }

        virtual const char* ComponentName() = 0;

    protected:

        /** The unique ID of this component */
        ComponentID id = -1;

        /** If this component is enabled or not */
        bool isEnabled = true;

        /** The owner of this component */
        EntityID owner;

    };  // IComponent

}   // namespace ECS