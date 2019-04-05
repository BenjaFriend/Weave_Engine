#pragma once

// I used this as a reference for setting up ECS:
// https://github.com/tobias-stein/EntityComponentSystem

#include "stdafx.h"
#include "Resources/ISaveable.h"
#include "Utils/SaveFileDefs.h"
#include "json/json.hpp"
#include "MemoryBitStream.h"

/// <summary>
/// Add miscellaneous component meta data to a component class
/// </summary>
#define COMPONENT( name )                                                       \
    static const char* ClassName() { return #name; }                            \
    virtual char const* ComponentName() override { return ClassName();  }       \
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
    

class IEntity;

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
        virtual void DrawEditorGUI() {}

        /// <summary>
        /// Write the data for this component to a file
        /// </summary>
        /// <param name="aOutFile">The file stream to write to</param>
        virtual void SaveObject( nlohmann::json & aEntComponentArray );

        /// <summary>
        /// Write this component to a replicated bit stream
        /// </summary>
        /// <param name="inOutputStream"></param>
        /// <param name="inDirtyState"></param>
        virtual void Write( OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState ) const
        {
            ( void ) inOutputStream;
            ( void ) ( inDirtyState );
        }

        /// <summary>
        /// Read this component from a bit stream
        /// </summary>
        /// <param name="inInputStream"></param>
        virtual void Read( InputMemoryBitStream& inInputStream )
        {
            ( void ) ( inInputStream );
        }

        ////////////////////////////////////////////////////    
        // Operators 

        inline const bool operator==( const IComponent& other ) const { return id == other.id; }
        inline const bool operator!=( const IComponent& other ) const { return id == other.id; }

        /////////////////////////////////////////////////    
        // Accessors 

        FORCE_INLINE const ComponentID GetComponentId() const { return this->id; }

        FORCE_INLINE const bool IsEnabled() const { return this->isEnabled; }

        FORCE_INLINE void SetEnabled( bool aEnabledState )
        {
            this->isEnabled = aEnabledState;
            ( this->isEnabled ? OnEnable() : OnDisable() );
        }

        /// <summary>
        /// Get the owning Entity ID of this component
        /// </summary>
        /// <returns>Entity ID of the owning entity</returns>
        FORCE_INLINE const size_t& GetOwner() const { return this->owner; }

        FORCE_INLINE IEntity* GetEntity() const { return OwningEntity; }

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
        ComponentID id = 0;

        /** If this component is enabled or not */
        bool isEnabled = true;

        /** The owner of this component */
        size_t owner = 0;

        IEntity* OwningEntity = nullptr;

        virtual void OnEnable() {}

        virtual void OnDisable() {}




    };  // IComponent

}   // namespace ECS