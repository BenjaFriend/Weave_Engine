#pragma once

#include "stdafx.h"
#include "Entity/IEntity.h"
#include "../ECS/ComponentManager.h"
#include "../Resources/ISaveable.h"
#include "Transform.h"
#include "Utils/SaveFileDefs.h"

/////////////////////////////////////////////////
// Forward Declarations
class Component;

/// <summary>
///  Represents a game entity and provides access to their components
/// </summary>
/// <author>Ben Hoffman</author>
class Entity : public ISaveable, public IEntity
{

    static size_t EntityCount;

public:

    /// <summary>
    /// Default constructor for this entity
    /// </summary>
    Entity();

    /// <summary>
    /// Creates an empty game object with the given name
    /// </summary>
    /// <param name="aName">Name of this entity</param>
    Entity( std::string aName );

    virtual ~Entity();

    /// <summary>
    /// Create this entity based off of a scene file data set
    /// </summary>
    /// <param name="aFile">The set of data about this entity</param>
    Entity* ConstructFromFile( nlohmann::json const & aFile );

    // Components ------------------------------

    /// <summary>
    /// Get a pointer to this component on this entity
    /// </summary>
    /// <returns>
    /// Pointer to the component if it exists, nullptr 
    /// if this entity does not have a component of this type
    /// </returns>
    template<typename T>
    T* GetComponent()
    {
        return this->componentManager->GetComponent<T>( this->entID );
    }

    /// <summary>
    /// Add a component of type T to this entity
    /// </summary>
    template<class T, class ...P>
    T* AddComponent( P&&... param )
    {
        return
            this->componentManager->AddComponent<T>(
                this,
                std::forward<P>( param )...
                );
    }

    /// <summary>
    /// Remove the component of this type from this entity
    /// </summary>
    template<typename T>
    void RemoveComponent()
    {
        this->componentManager->RemoveComponent<T>( this->entID );
    }

    FORCE_INLINE const ECS::ComponentMap * GetAllComponents() const
    {
        return componentManager->GetAllComponents( this->entID );
    }

    FORCE_INLINE void RemoveAllComponents() { componentManager->RemoveAllEntityComponents( entID ); }

    /// <summary>
    /// Save this entity's data and components into a json array of 
    /// entities
    /// </summary>
    /// <param name="aJsonEntityArray">nlohmann::json::array of entities</param>
    void SaveObject( nlohmann::json& aJsonEntityArray );

    /// <summary>
    /// Reset this entity for re-use in the next scene.
    /// </summary>
    void Reset();

private:

    /** handles the adding/removing of components for this entity */
    ECS::ComponentManager * componentManager = nullptr;

    /** The transform of the entity */
    Transform* EntityTransform = nullptr;

    ////////////////////////////////////////////////////
    // Accessors
    ////////////////////////////////////////////////////
public:

    /** Get the current transform of this object */
    FORCE_INLINE Transform* GetTransform() const { return EntityTransform; }

};