#pragma once

// I used this as a reference for setting up ECS:
// https://github.com/tobias-stein/EntityComponentSystem

#include "../stdafx.h"
#include "../Resources/ISaveable.h"

#if defined( ENABLE_UI )

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#endif

namespace ECS
{
    using ComponentID = size_t;
    using ComponentTypeId = size_t;
    using EntityID = size_t;

    /// <summary>
    /// Base component interface for all components in ECS
    /// </summary>
    class IComponent : ISaveable
    {
        friend class ComponentManager;

    public:

        IComponent() {}

        virtual ~IComponent() {}

        /// <summary>
        /// Natively draw any IMGUI fields that are to be used
        /// with this component here
        /// </summary>
        virtual void DrawEditorGUI() = 0;
        
        /// <summary>
        /// Write the data for this component to a file
        /// </summary>
        /// <param name="aOutFile">The file stream to write to</param>
        virtual void SaveObject( nlohmann::json & aOutFile ) = 0;

        ////////////////////////////////////////////////////    
        // Operators 

        inline const bool operator==( const IComponent& other ) const { return id == other.id; }
        inline const bool operator!=( const IComponent& other ) const { return id == other.id; }

        /////////////////////////////////////////////////    
        // Accessors 

        inline const ComponentID GetComponentId() const { return this->id; }

        inline const bool IsEnabled() const { return this->isEnabled; }

        inline void SetSenabled( bool aEnabledState ) 
        { 
            this->isEnabled = aEnabledState; 
            ( this->isEnabled ? OnEnable() : OnDisable() );
        }

        /// <summary>
        /// Get the owning Entity ID of this component
        /// </summary>
        /// <returns>Entity ID of the owning entity</returns>
        const EntityID& GetOwner() const { return this->owner; }

        /// <summary>
        /// Get the human-readable name of this component
        /// </summary>
        /// <returns>const char* of what this component should be called</returns>
        virtual const char* ComponentName() = 0;

    protected:

        /** The unique ID of this component */
        ComponentID id;

        /** If this component is enabled or not */
        bool isEnabled = true;

        /** The owner of this component */
        EntityID owner;

        virtual void OnEnable() {}

        virtual void OnDisable() {}

    };  // IComponent

}   // namespace ECS