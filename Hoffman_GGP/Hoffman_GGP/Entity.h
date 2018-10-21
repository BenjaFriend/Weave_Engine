#pragma once
#include "DebugSettings.h"

#include <DirectXMath.h>	// XMFLOAT3, XMFLOAT4X4

/////////////////////////////////////////////////
// Forward Declarations
class Mesh;
class Material;

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
    Entity( Mesh * aMesh, Material* aMat );

    /** Virtual destructor for this entity */
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

    ////////////////////////////////////////////////////
    // Accessors
    ////////////////////////////////////////////////////

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

};

