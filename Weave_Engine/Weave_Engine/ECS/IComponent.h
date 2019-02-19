#pragma once

// I used this as a reference for setting up ECS:
// https://github.com/tobias-stein/EntityComponentSystem

#include "../stdafx.h"
#include "../Resources/ISaveable.h"
#include "../Utils/SaveFileDefs.h"
#include "json/json.hpp"

#if defined( ENABLE_UI )

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#endif

/// <summary>
/// Add miscellaneous component meta data to a component class
/// </summary>
#define COMPONENT( name )                                               \
    static const char* ClassName() { return #name; }                    \
    virtual char const* ComponentName() { return ClassName();  }        \
    const static ECS::IComponent::ConcreteFactory< name > CompFactory; 

/// <summary>
/// Initalize misc. static members of the component in the CPP file, so that
/// they are added to the proper factory patterns at compile time. 
/// </summary>
#define  COMPONENT_INIT( name )                                         \
    const static ECS::IComponent::ConcreteFactory< name > CompFactory = {};


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

        virtual ComponentTypeId GetStaticComponentTypeID() const = 0;

        /// <summary>
        /// Natively draw any IMGUI fields that are to be used
        /// with this component here
        /// </summary>
        virtual void DrawEditorGUI() = 0;

        /// <summary>
        /// Write the data for this component to a file
        /// </summary>
        /// <param name="aOutFile">The file stream to write to</param>
        virtual void SaveObject( nlohmann::json & aEntComponentArray )
        {
            nlohmann::json comp_data = nlohmann::json::object();
            comp_data [ COMP_SAVE_KEY ] = ComponentName();

            SaveComponentData( comp_data );

            if ( aEntComponentArray.is_array() )
            {
                aEntComponentArray.push_back( comp_data );
            }
            else
            {
                LOG_WARN( "Could not save component data to the array!" );
            }
        }

        ////////////////////////////////////////////////////    
        // Operators 

        inline const bool operator==( const IComponent& other ) const { return id == other.id; }
        inline const bool operator!=( const IComponent& other ) const { return id == other.id; }

        /////////////////////////////////////////////////    
        // Accessors 

        inline const ComponentID GetComponentId() const { return this->id; }

        inline const bool IsEnabled() const { return this->isEnabled; }

        inline void SetEnabled( bool aEnabledState )
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

        static IComponent* ReadFromFile( nlohmann::json & aInitData );

        class Factory
        {
        protected:
            Factory( std::string const& type );
        public:
            virtual IComponent* constructFromFile( nlohmann::json const & source ) const = 0;
        };

        template <typename Derived>
        class ConcreteFactory : public Factory
        {
        public:
            ConcreteFactory() : Factory( Derived::ClassName() ) {}
            virtual IComponent* constructFromFile( nlohmann::json const & source ) const
            {
                return new Derived( source );
            }
        };

        typedef std::map <std::string, Factory const*> FactoryMap;
        static FactoryMap & ComponentFactories()
        {
            static FactoryMap theOneAndOnly;
            return theOneAndOnly;
        }

    protected:

        virtual void SaveComponentData( nlohmann::json & aCompDataObj ) {}

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