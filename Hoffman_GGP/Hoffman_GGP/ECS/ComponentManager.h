#pragma once

#include <vector>
#include <unordered_map>

#include "IComponent.h"

namespace ECS
{

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
        T* AddComponent( EntityID aEntityID, ARGS&&... args )
        {
            IComponent* newComponent = new T( std::forward<ARGS>( args )... );

            const ComponentTypeId CTID = T::STATIC_COMPONENT_TYPE_ID;

            newComponent->owner = aEntityID;
            newComponent->id = ComponentCount;

            ++ComponentCount;

            this->activeComponents[ aEntityID ][ CTID ] = newComponent;

            return static_cast< T* >( newComponent );
        }

        template <class T>
        T* GetComponent( const EntityID aEntityID )
        {
            const ComponentTypeId CTID = T::STATIC_COMPONENT_TYPE_ID;

            return static_cast< T* >( activeComponents[ aEntityID ][ CTID ] );
        }

        template <class T>
        void RemoveComponent( const EntityID aEntityID )
        {
            const ComponentTypeId CTID = T::STATIC_COMPONENT_TYPE_ID;

            delete ( activeComponents[ aEntityID ][ CTID ] );
            activeComponents[ aEntityID ][ CTID ] = nullptr;
        }

    private:

        ComponentManager();

        ~ComponentManager();

        /** Static instance of this component manager */
        static ComponentManager* instance;

        /// <summary>
        /// Delete all active components
        /// </summary>
        void CleanupAllComponents();

        // This has pretty bad spatial locality but I don't have time
        // to implement a custom allocator
        typedef std::unordered_map<ComponentTypeId, IComponent*> ComponentMap;

        /** Keep track of all component types */
        std::unordered_map<EntityID, ComponentMap> activeComponents;

        /** Current count of all components */
        size_t ComponentCount = 0;

    };  // ComponentManager




} // namespace ECS