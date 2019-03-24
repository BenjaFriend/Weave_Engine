#pragma once

#include "stdafx.h"

#include "../ECS/ComponentManager.h"
#include "../Resources/ISaveable.h"
#include "Transform.h"
#include "Utils/SaveFileDefs.h"

/////////////////////////////////////////////////
// Forward Declarations
class Mesh;
class Material;
class Component;

/// <summary>
///  Represents a game entity and provides access to their components
/// </summary>
/// <author>Ben Hoffman</author>
class Entity : ISaveable
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

    ~Entity();

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

    /** The unique ID of this entity */
    ECS::EntityID entID;

    /** Flag for if this entity is active or not */
    UINT32 IsActive : 1;

    /** If true, then this entity will get destroyed when  */
    UINT32 IsDestroyableOnLoad : 1;

    /** If true, then this entity has been initialized and is valid in the memory pool */
    UINT32 IsValid : 1;

    /** The name of this object */
    std::string Name = "Default Entity";

    ////////////////////////////////////////////////////
    // Accessors
    ////////////////////////////////////////////////////
public:

    /** Get the current transform of this object */
    FORCE_INLINE Transform* GetTransform() const { return EntityTransform; }

    /// <summary>
    /// If an entity is destroyable on load, then it will be deleted during a 
    /// scene change. If not, then it will remain persistent throughout scenes
    /// </summary>
    /// <returns>True if destroyable</returns>
    FORCE_INLINE const bool GetIsDestroyableOnLoad() const { return IsDestroyableOnLoad; }

    FORCE_INLINE void SetIsDestroyableOnLoad( const bool aVal ) { IsDestroyableOnLoad = aVal; }

    FORCE_INLINE const bool GetIsValid() const { return IsValid; }

    FORCE_INLINE void SetIsValid( const bool aValid ) { IsValid = aValid; }

    FORCE_INLINE const ECS::EntityID GetID() const { return this->entID; }

    /// <summary>
    /// Sets if this entity is active or not
    /// </summary>
    /// <param name="aStatus">True if active, false if in-active</param>
    FORCE_INLINE void SetIsActive( const bool aStatus ) { IsActive = aStatus; }

    /// <summary>
    /// Get if this entity is active or not
    /// </summary>
    /// <returns>True if active, false if in-active</returns>
    FORCE_INLINE const bool GetIsActive() const { return IsActive; }

    /// <summary>
    /// Get this entity's name
    /// </summary>
    /// <returns>Reference to the name of this entity</returns>
    FORCE_INLINE const std::string & GetName() const { return Name; }

    /// <summary>
    /// Set the name of this entity
    /// </summary>
    /// <param name="newName">The new name of this entity</param>
    FORCE_INLINE void SetName( std::string newName ) { Name = newName; }

};