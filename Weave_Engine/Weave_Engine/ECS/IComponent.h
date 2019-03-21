#pragma once

// I used this as a reference for setting up ECS:
// https://github.com/tobias-stein/EntityComponentSystem

#include "stdafx.h"
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

#define REMOVE_COMP_BTN( name )                                             \
    if( ImGui::Button("Remove Component") )                                 \
    { OwningEntity->RemoveComponent< name > (); return; }               
    

class Entity;
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
        virtual void SaveObject( nlohmann::json & aEntComponentArray );

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

        const Entity* GetEntity() const { return OwningEntity; }

        /// <summary>
        /// Get the human-readable name of this component
        /// </summary>
        /// <returns>const char* of what this component should be called</returns>
        virtual const char* ComponentName() = 0;

        /// <summary>
        /// Construct this component at runtime from a scene serialization file
        /// </summary>
        /// <param name="aInitData">The initalization data for this component</param>
        /// <returns>Pointer to the newly constructed component</returns>
        static IComponent* ReadFromFile( nlohmann::json & aInitData );

        /// <summary>
        /// Construct this component at runtime from the editor
        /// </summary>
        /// <param name="aCompName">The class name of this component</param>
        /// <returns>Pointer to newly constructed component</returns>
        static IComponent* ReadFromEditor( const std::string & aCompName );

#pragma region Component Factory
        /// <summary>
        /// Base factory class for components to be added at runtime. 
        /// </summary>
        class Factory
        {
        protected:
            Factory( std::string const& type );
        public:
            virtual IComponent* ConstructFromFile( nlohmann::json const & source ) const = 0;
            virtual IComponent* ConstructFromEditor() const = 0;
        };

        /// <summary>
        /// A component factory that will create a new factory based on it's class name 
        /// for use with the editor and scene serialization. 
        /// </summary>
        template <typename Derived>
        class ConcreteFactory : public Factory
        {
        public:

            ConcreteFactory() : Factory( Derived::ClassName() ) {}

            virtual IComponent* ConstructFromFile( nlohmann::json const & source ) const
            {
                return new Derived( source );
            }

            virtual IComponent* ConstructFromEditor()const
            {
                return new Derived();
            }
        };

        typedef std::map <std::string, Factory const*> FactoryMap;
        static FactoryMap & ComponentFactories()
        {
            static FactoryMap theOneAndOnly;
            return theOneAndOnly;
        }
#pragma endregion 

    protected:

        virtual ~IComponent() {}

        virtual void SaveComponentData( nlohmann::json & aCompDataObj ) {}

        /** The unique ID of this component */
        ComponentID id;

        /** If this component is enabled or not */
        bool isEnabled = true;

        /** The owner of this component */
        EntityID owner;

        Entity* OwningEntity = nullptr;

        virtual void OnEnable() {}

        virtual void OnDisable() {}




    };  // IComponent

}   // namespace ECS