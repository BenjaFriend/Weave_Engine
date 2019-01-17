#pragma once

#include "../stdafx.h"

#include "../Physics/Collisions.h"
#include "../ECS/ComponentManager.h"
#include "../Resources/ISaveable.h"
#include "Transform.h"

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
    /// Constructor for this entity
    /// </summary>
    /// <param name="aMesh">A pointer to this entity's mesh</param>
    /// <param name="aMat">A material for this entity to use</param>
    Entity( Mesh * aMesh, Material* aMat, std::string aName );

    /// <summary>
    /// Default constructor for this entity
    /// </summary>
    Entity();

    ~Entity();

    /// <summary>
    /// Move the entity relative to the direction it is facing 
    /// </summary>
    /// <param name="aX">new X value to add to this position</param>
    /// <param name="aY">new Y value to add to this position</param>
    /// <param name="aZ">new Z value to add to this position</param>
    void MoveRelative( const float aX, const float aY, const float aZ );

    /// <summary>
    /// Sets the material's shader data and activates the shaders
    /// </summary>
    /// <param name="aView">View matrix</param>
    /// <param name="aProjection">Project matrix</param>
    void PrepareMaterial( const VEC4x4 & aView, const VEC4x4 & aProjection );

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

    void SaveObject( nlohmann::json& aOutFile );

private:

    /** The transform of the entity */
    Transform* EntityTransform = nullptr;

    // TODO: Make this a smart pointer at some point
    /** This entity's mesh. Just use a reference to a pointer so that we can do instance meshes */
    Mesh* EntityMesh = nullptr;

    /** This entity's material */
    Material* EntityMaterial = nullptr;

    /** The current position of this entity */
    DirectX::XMFLOAT3 Position;

    /** The current rotation of this entity */
    DirectX::XMFLOAT4 Rotation;

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

    /** Returns this entity's mesh */
    Mesh * GetEntityMesh() const;

    /** Returns this entity's material */
    const Material* GetMaterial() const;

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