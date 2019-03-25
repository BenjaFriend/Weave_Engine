#pragma once

#include <vector>
#include <unordered_map>

#include "ECS/IComponent.h"
#include "Entity/IEntity.h"

namespace ECS
{

    // This has pretty bad spatial locality but I don't have time
    // to implement a custom allocator
    typedef std::unordered_map<ComponentTypeId, IComponent*> ComponentMap;

    /// <summary>
    /// Component manager that keeps track of what components
    /// currently exist
    /// </summary>
    class ComponentManager
    {
        friend class IComponent;

    public:

        /// <summary>
        /// Get a pointer to the instance of the 
        /// Component manager
        /// </summary>
        /// <returns>Pointer to Component manager</returns>
        static ComponentManager* GetInstance();

        /// <summary>
        /// Destroy the current instance of the component manager
        /// and all active instances of components
        /// </summary>
        static void ReleaseInstance();

        ////////////////////////////////////////////
        // Operators 

        ComponentManager( const ComponentManager& ) = delete;
        ComponentManager& operator=( ComponentManager& ) = delete;

        /// <summary>
        /// Add a component to the given Entity
        /// </summary>
        /// <param name="aEntityID">The entity to put this component on</param>
        /// <param name="...args">Arguments for your component's constructor</param>
        /// <returns>Pointer to the newly created component</returns>
        template<class T, class ...ARGS>
        T* AddComponent( IEntity* aEntity, ARGS&&... args )
        {
            assert( aEntity != nullptr );
            const ComponentTypeId CTID = T::STATIC_COMPONENT_TYPE_ID;

            const ECS::EntityID aEntityID = aEntity->GetID();

            // Make sure that this component doesn't exist already
            assert( this->activeComponents [ aEntityID ] [ CTID ] == nullptr );

            IComponent* newComponent = new T( std::forward<ARGS>( args )... );

            newComponent->owner = aEntityID;
            newComponent->id = ComponentCount;
            newComponent->OwningEntity = aEntity;

            ++ComponentCount;

            // Map the component
            this->activeComponents [ aEntityID ] [ CTID ] = newComponent;

            return static_cast< T* >( newComponent );
        }

        /// <summary>
        /// Add a component to the given entity from a json data object
        /// </summary>
        /// <param name="aEntityID">The entity to add to</param>
        /// <param name="aCompData">The json component data for creating a </param>
        /// <returns>Returns true if successfully added</returns>
        bool AddComponent( IEntity* aEntity, nlohmann::json & aCompData );

        template <class T>
        T* GetComponent( const EntityID aEntityID )
        {
            const ComponentTypeId CTID = T::STATIC_COMPONENT_TYPE_ID;

            return static_cast< T* >( activeComponents [ aEntityID ] [ CTID ] );
        }

        /// <summary>
        /// Add a component from the editor
        /// </summary>
        /// <param name="aEntity">The entity to add this component to</param>
        /// <param name="aCompName">Class name of the component</param>
        /// <returns>True if success, false if failure</returns>
        bool AddComponentFromEditor( IEntity* aEntity, const std::string & aCompName );

        template <class T>
        T* FindComponentOfType()
        {
            const ComponentTypeId CTID = T::STATIC_COMPONENT_TYPE_ID;

            std::unordered_map<EntityID, ComponentMap>::iterator it;

            for ( it = activeComponents.begin(); it != activeComponents.end(); ++it )
            {
                if ( it->second.find( CTID ) != it->second.end() )
                {
                    return reinterpret_cast< T* >( it->second [ CTID ] );
                }
            }

            return nullptr;
        }

        const ComponentMap * GetAllComponents( const EntityID aEntityID ) const
        {
            auto itr = activeComponents.find( aEntityID );
            if ( itr == activeComponents.end() )
            {
                // not found
                return nullptr;
            }
            else
            {
                return &itr->second;
            }
        }

        template <class T>
        void RemoveComponent( const EntityID aEntityID )
        {
            const ComponentTypeId CTID = T::STATIC_COMPONENT_TYPE_ID;
            assert( activeComponents [ aEntityID ] [ CTID ] != nullptr );

            delete ( activeComponents [ aEntityID ] [ CTID ] );
            activeComponents [ aEntityID ] [ CTID ] = nullptr;
            --ComponentCount;
        }

        /// <summary>
        ///  Remove all the components that are on this eneity, and remove the 
        /// entity from this map 
        /// </summary>
        /// <param name="aEntityID"></param>
        void RemoveAllEntityComponents( const EntityID aEntityID );

    private:

        ComponentManager();

        ~ComponentManager();

        /** Static instance of this component manager */
        static ComponentManager* instance;

        /// <summary>
        /// Delete all active components
        /// </summary>
        void CleanupAllComponents();

        /** Keep track of all component types */
        std::unordered_map< EntityID, ComponentMap  > activeComponents;

        /** Current count of all components */
        size_t ComponentCount = 0;

    };  // ComponentManager


} // namespace ECS