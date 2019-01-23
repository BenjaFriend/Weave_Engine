#pragma once

#include "../ECS/Component.h"
#include "../MathHelper.h"

/// <summary>
/// A transform component will give entities a position, 
/// rotation, and scale
/// </summary>
/// <author>Ben Hoffman</author>
class Transform : public ECS::Component<Transform>
{
public:
    
    Transform();

    ~Transform();

    virtual void DrawEditorGUI() override;

    virtual void SaveObject( nlohmann::json & aOutFile ) override;

    virtual const char* ComponentName() override { return "Transform"; }

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

    /** Returns this entity's current position */
    const glm::vec3 & GetPosition() const;

    /// <summary>
    /// Set the position of this entity
    /// </summary>
    /// <param name="aNewPos">The new position of this object</param>
    void SetPosition( const glm::vec3 & aNewPos );

    /// <summary>
    /// Set the position of this entity 
    /// </summary>
    /// <param name="aX">new X value of the position</param>
    /// <param name="aY">new Y value of the position</param>
    /// <param name="aZ">new Z value of the position</param>
    void SetPosition( const float aX, const float aY, const float aZ );

    void SetPosX( const float aVal ) { Position.x = aVal; }
    void SetPosY( const float aVal ) { Position.y = aVal; }
    void SetPosZ( const float aVal ) { Position.z = aVal; }

    /// <summary>
    /// Get the current scale 
    /// </summary>
    /// <returns>Reference to the current scale</returns>
    const glm::vec3 & GetScale() const;

    /// <summary>
    /// Set the position of this entity 
    /// </summary>
    /// <param name="aNewScale">The new scale of this object</param>
    void SetScale( const glm::vec3 & aNewScale );

    /// <summary>
    /// Set the scale of the this entity 
    /// </summary>
    /// <param name="aX">new X value of the scale</param>
    /// <param name="aY">new Y value of the scale</param>
    /// <param name="aZ">new Z value of the scale</param>
    void SetScale( const float aX, const float aY, const float aZ );

    void SetScaleX( const float aVal ) { Scale.x = aVal; }
    void SetScaleY( const float aVal ) { Scale.y = aVal; }
    void SetScaleZ( const float aVal ) { Scale.z = aVal; }

    /// <summary>
    /// Get the current rotation
    /// </summary>
    /// <returns>Reference to the current rotation</returns>
    const glm::vec3 & GetRotation() const;

    /// <summary>
    /// Set the rotation of this entity 
    /// </summary>
    /// <param name="aNewRot">The new rotation of this object</param>
    void SetRotation( const glm::vec3 & aNewRot );

    /// <summary>
    /// Calculate the world matrix for this entity
    /// </summary>
    /// <returns>The calculate world matrix for this entity</returns>
    const glm::mat4 GetWorldMatrix() const;

private:
    
    /** The current position */
    glm::vec3 Position;

    /** The current scale */
    glm::vec3 Scale;

    /** Euler rotation of this entity */
    glm::vec3 Rotation;
};