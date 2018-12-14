#pragma once

#include "../stdafx.h"

#include "../ECS/Component.h"

/// <summary>
/// Box collider component
/// </summary>
class BoxCollider : public ECS::Component<BoxCollider>
{
public:

    BoxCollider( const VEC3 & aExtents = VEC3( 1.f, 1.f, 1.f ) );

    ~BoxCollider();

    virtual void DrawEditorGUI() override;

    virtual void SaveObject( nlohmann::json & aOutFile ) override;

    virtual const char* ComponentName() { return "BoxCollider"; }

    ////////////////////////////////////////////////////
    // Accessors
    ////////////////////////////////////////////////////

    const bool Collides( const BoxCollider & aOther );

    void SetCenterOffset( const VEC3 & aVal );

    const VEC3 & GetCenterOffset() const;

    void SetExtents( const VEC3 & aVal );

    const VEC3 & GetExtents() const;

    const bool SetIsTrigger() const;

    void SetIsTrigger( bool aVal );

private:

    VEC3 CenterOffset;

    VEC3 Extents;

    bool IsTrigger = false;

};