#pragma once

#include "../stdafx.h"

#include "../ECS/ComponentManager.h"
#include "../Resources/ISaveable.h"
#include "Transform.h"

/////////////////////////////////////////////////
// Savable key definitions 
#include "../Utils/SaveFileDefs.h"

/////////////////////////////////////////////////
// Forward Declarations
class Mesh;
class Material;
class Component;

/// <summary>
///  Represents a game entity and their transformations.
/// </summary>
/// <author>Ben Hoffman</author>
class Entity : ISaveable
{

    static size_t EntityCount;

public:

    /// <summary>
    /// Creates an empty game object with the given name
    /// </summary>
    /// <param name="aName">Name of this entity</param>
    Entity( std::string aName );

    /// <summary>
    /// Creates an empty game object with the given name at the 
    /// given position
    /// </summary>
    /// <param name="aName">Name of this entity</param>
    /// <param name="aPos">Position of this entity</param>
    Entity( std::string aName, glm::vec3 aPos );

    /// <summary>
    /// Create this entity bsaed off of a scene file data set
    /// </summary>
    /// <param name="aFile">The set of data about this entity</param>
    Entity( nlohmann::json const & aFile );

    /// <summary>
    /// Default constructor for this entity
    /// </summary>
    Entity();

    ~Entity();

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
                this->entID,
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

    const ECS::ComponentMap * GetAllComponents() const
    {
        return componentManager->GetAllComponents( this->entID );
    }

    /// <summary>
    /// Save this entity's data and components into a json array of 
    /// entities
    /// </summary>
    /// <param name="aJsonEntityArray">nlohmann::json::array of entities</param>
    void SaveObject( nlohmann::json& aJsonEntityArray );

private:

    /** The transform of the entity */
    Transform* EntityTransform = nullptr;

    /** Flag for if this entity is active or not */
    bool IsActive;

    /** The name of this object */
    std::string Name = "Default Entity";

    /** The unique ID of this entity */
    ECS::EntityID entID;

    /** handles the adding/removing of components for this entity */
    ECS::ComponentManager * componentManager = nullptr;

    ////////////////////////////////////////////////////
    // Accessors
    ////////////////////////////////////////////////////
public:

    /** Get the current transform of this object */
    Transform* GetTransform() const { return EntityTransform; }

    /// <summary>
    /// Sets if this entity is active or not
    /// </summary>
    /// <param name="aStatus">True if active, false if in-active</param>
    void SetIsActive( const bool aStatus );

    /// <summary>
    /// Get if this entity is active or not
    /// </summary>
    /// <returns>True if active, false if in-active</returns>
    const bool GetIsActive() const;

    /// <summary>
    /// Get this entity's name
    /// </summary>
    /// <returns>Reference to the name of this entity</returns>
    const std::string & GetName() const;

    /// <summary>
    /// Set the name of this entity
    /// </summary>
    /// <param name="newName">The new name of this entity</param>
    void SetName( std::string newName );

};