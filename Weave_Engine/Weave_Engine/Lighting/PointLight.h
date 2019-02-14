#pragma once

#include "../stdafx.h"

#include "LightShaderDefs.h"
#include "../ECS/Component.h"

class LightSystem;

class PointLight : public ECS::Component<PointLight>
{
public:

    COMP_NAME( "PointLight" );

    PointLight( 
        LightSystem* aRendSys,
        glm::vec3 aColor,
        glm::vec3 aPos, 
        float aIntensity = 1.f, 
        float aRange = 5.f
    );

    ~PointLight();

    void SetColor( glm::vec3 & aColor );

    void SetPosition( glm::vec3 aPos );

    void SetIntensity( float aVal );

    void SetRange( float aVal );

    const PointLightData& GetLightData() const;

    const bool GetDrawRange() const;

    void SetDrawRange(bool aDrawRange);

    virtual void DrawEditorGUI() override;

protected:

    virtual void SaveComponentData( nlohmann::json & aOutFile ) override;

private:

    PointLightData LightingData;

    bool DrawRange = false;

};