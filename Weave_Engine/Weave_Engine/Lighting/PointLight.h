#pragma once

#include "stdafx.h"

#include "LightShaderDefs.h"
#include "ECS/Component.h"
#include "UI_OPTIONS.h"

class PointLight : public ECS::Component<PointLight>
{
public:

    COMPONENT( PointLight );

    PointLight();

    PointLight( 
        glm::vec3 aColor,
        glm::vec3 aPos, 
        float aIntensity = 1.f, 
        float aRange = 5.f
    );

    PointLight( PointLightData aData, const glm::vec3 & aPosOffset );

    PointLight( nlohmann::json const & aInitData );

    ~PointLight();

    void SetColor( glm::vec3 & aColor );

    void SetPosition( glm::vec3 aPos );

    void SetIntensity( float aVal );

    void SetRange( float aVal );

    const PointLightData& GetLightData();

    const bool GetDrawRange() const;

    void SetDrawRange(bool aDrawRange);

    virtual void DrawEditorGUI() override;

protected:

    virtual void SaveComponentData( nlohmann::json & aOutFile ) override;

private:

    PointLightData LightingData;

    /** the Offset position from the owning entity */
    glm::vec3 PosOffset = {};

    bool DrawRange = false;

};