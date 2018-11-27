#pragma once

#include "../stdafx.h"

#include <DirectXMath.h>	// XMFLOAT3, XMFLOAT4X4
#include "../Physics/Collisions.h"
#include "../ECS/ComponentManager.h"

/////////////////////////////////////////////////
// Forward Declarations
class Mesh;
class Material;

enum EPhysicsLayer
{
    MOVEABLE,
    STATIC
};

/// <summary>
///  Represents a game entity and their transformations.
/// </summary>
/// <author>Ben Hoffman</author>
class Entity
{

public:

    /// <summary>
    /// Constructor for this entity
    /// </summary>
    /// <param name="aMesh">A pointer to this entity's mesh</param>
    /// <param name="aMat">A material for this entity to use</param>
    Entity( Mesh * aMesh, Material* aMat, std::string aName );

    ~Entity();

    /// <summary>
    /// Move the entity relative to the direction it is facing 
    /// </summary>
    /// <param name="aX">new X value to add to this position</param>
    /// <param name="aY">new Y value to add to this position</param>
    /// <param name="aZ">new Z value to add to this position</param>
    void MoveRelative( const float aX, const float aY, const float aZ );

    /// <summary>
    /// Adds to the position NOT relative to the direction you are facing
    /// </summary>
    /// <param name="aX">new X value to add to this position</param>
    /// <param name="aY">new Y value to add to this position</param>
    /// <param name="aZ">new Z value to add to this position</param>
    void MoveAbsolute( const float aX, const float aY, const float aZ );

    /// <summary>
    /// Sets the material's shader data and activates the shaders
    /// </summary>
    /// <param name="aView">View matrix</param>
    /// <param name="aProjection">Project matrix</param>
    void PrepareMaterial( const DirectX::XMFLOAT4X4 & aView, const DirectX::XMFLOAT4X4 & aProjection );

    /// <summary>
    /// Apply a force to this entity
    /// </summary>
    /// <param name="aForce">The force to apply</param>
    void ApplyForce( const DirectX::XMFLOAT3 aForce );

    /// <summary>
    /// Apply the current acceleration to the velocity and set acceleration to 0
    /// </summary>
    void ApplyAcceleration();

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

private:

    // TODO: Make this a smart pointer at some point
    /** This entity's mesh. Just use a reference to a pointer so that we can do instance meshes */
    Mesh* EntityMesh = nullptr;

    /** This entity's material */
    Material* EntityMaterial = nullptr;

    /** The current position of this entity */
    DirectX::XMFLOAT3 Position;

    /** The current Scale of this entity */
    DirectX::XMFLOAT3 Scale;

    /** The current rotation of this entity */
    DirectX::XMFLOAT4 Rotation;

    /** Flag for if this entity is active or not */
    bool IsActive;

    /** The current physics layer of this entity */
    EPhysicsLayer PhysicsLayer = EPhysicsLayer::MOVEABLE;

    /** The mass of this object */
    float Mass = 1.0f;

    DirectX::XMFLOAT3 Velocity;

    DirectX::XMFLOAT3 Acceleration;

    Physics::BoxCollider Collider;

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

    /** Returns this entity's current position */
    const DirectX::XMFLOAT3 & GetPosition() const;

    /// <summary>
    /// Set the position of this entity
    /// </summary>
    /// <param name="aNewPos">The new position of this object</param>
    void SetPosition( const DirectX::XMFLOAT3 & aNewPos );

    /// <summary>
    /// Set the position of this entity 
    /// </summary>
    /// <param name="aX">new X value of the position</param>
    /// <param name="aY">new Y value of the position</param>
    /// <param name="aZ">new Z value of the position</param>
    void SetPosition( const float aX, const float aY, const float aZ );

    /** Returns this entity's current scale */
    const DirectX::XMFLOAT3 & GetScale() const;

    /// <summary>
    /// Set the position of this entity 
    /// </summary>
    /// <param name="aNewScale">The new scale of this object</param>
    void SetScale( const DirectX::XMFLOAT3 & aNewScale );

    /// <summary>
    /// Set the scale of the this entity 
    /// </summary>
    /// <param name="aX">new X value of the scale</param>
    /// <param name="aY">new Y value of the scale</param>
    /// <param name="aZ">new Z value of the scale</param>
    void SetScale( const float aX, const float aY, const float aZ );

    /** Returns this entity's current rotation */
    const DirectX::XMFLOAT4 & GetRotation() const;

    /// <summary>
    /// Set the rotation of this entity 
    /// </summary>
    /// <param name="aNewRot">The new rotation of this object</param>
    void SetRotation( const DirectX::XMFLOAT4 & aNewRot );

    /**
    * Calculate the world matrix for this entity
    *
    * @return	The calculate world matrix for this entity
    */
    DirectX::XMFLOAT4X4 GetWorldMatrix();

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

    // Physics -------------------------------

    const EPhysicsLayer GetPhysicsLayer() const;

    void SetPhysicsLayer( const EPhysicsLayer aLayer );

    void SetVelocity( const DirectX::XMFLOAT3& aVel );

    const DirectX::XMFLOAT3 GetVelocity() const;
    
    const float GetMass() const;

    void SetMass( const float aMass );

    const Physics::BoxCollider & GetCollider() const;

};