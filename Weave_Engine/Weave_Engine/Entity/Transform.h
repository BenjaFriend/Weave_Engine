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

    COMPONENT( Transform );

    Transform();

    Transform( nlohmann::json const & aInitData );

    ~Transform();

    virtual void DrawEditorGUI() override;

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
    inline const glm::vec3 & GetPosition() const { return Position; }

    /// <summary>
    /// Set the position of this entity
    /// </summary>
    /// <param name="aNewPos">The new position of this object</param>
    inline void SetPosition( const glm::vec3 & aNewPos ) { Position = aNewPos; }

    /// <summary>
    /// Set the position of this entity 
    /// </summary>
    /// <param name="aX">new X value of the position</param>
    /// <param name="aY">new Y value of the position</param>
    /// <param name="aZ">new Z value of the position</param>
    void SetPosition( const float aX, const float aY, const float aZ );

    inline void SetPosX( const float aVal ) { Position.x = aVal; }
    inline void SetPosY( const float aVal ) { Position.y = aVal; }
    inline void SetPosZ( const float aVal ) { Position.z = aVal; }

    /// <summary>
    /// Get the current scale 
    /// </summary>
    /// <returns>Reference to the current scale</returns>
    const glm::vec3 & GetScale() const { return Scale; }

    /// <summary>
    /// Set the position of this entity 
    /// </summary>
    /// <param name="aNewScale">The new scale of this object</param>
    inline void SetScale( const glm::vec3 & aNewScale ) { Scale = aNewScale; }

    /// <summary>
    /// Set the scale of the this entity 
    /// </summary>
    /// <param name="aX">new X value of the scale</param>
    /// <param name="aY">new Y value of the scale</param>
    /// <param name="aZ">new Z value of the scale</param>
    inline void SetScale( const float aX, const float aY, const float aZ );

    inline void SetScaleX( const float aVal ) { Scale.x = aVal; }
    inline void SetScaleY( const float aVal ) { Scale.y = aVal; }
    inline void SetScaleZ( const float aVal ) { Scale.z = aVal; }

    /// <summary>
    /// Get the current rotation
    /// </summary>
    /// <returns>Reference to the current rotation</returns>
    inline const glm::vec3 & GetRotation() const { return Rotation; }

    /// <summary>
    /// Set the rotation of this entity 
    /// </summary>
    /// <param name="aNewRot">The new rotation of this object</param>
    void SetRotation( const glm::vec3 & aNewRot ) { Rotation = aNewRot; }

    /// <summary>
    /// Calculate the world matrix for this entity
    /// </summary>
    /// <returns>The calculate world matrix for this entity</returns>
    const glm::mat4 GetWorldMatrix() const;

protected:

    virtual void SaveComponentData( nlohmann::json & aCompData ) override;

private:

    /** The current position */
    glm::vec3 Position;

    /** The current scale */
    glm::vec3 Scale;

    /** Euler rotation of this entity */
    glm::vec3 Rotation;
};