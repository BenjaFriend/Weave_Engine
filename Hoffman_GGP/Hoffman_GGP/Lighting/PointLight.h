#pragma once

#include "../stdafx.h"

#include <DirectXMath.h>

#include "LightShaderDefs.h"
#include "../ECS/Component.h"

class RenderSystem;

class PointLight : public ECS::Component<PointLight>
{
public:

    PointLight( 
        RenderSystem* aRendSys,
        DirectX::XMFLOAT3 aColor,
        DirectX::XMFLOAT3 aPos, 
        float aIntensity = 1.f, 
        float aRange = 5.f
    );

    ~PointLight();

    void SetColor( DirectX::XMFLOAT3 & aColor );

    void SetPosition( DirectX::XMFLOAT3 aPos );

    void SetIntensity( float aVal );

    void SetRange( float aVal );

    const PointLightData& GetLightData() const;

    virtual const char* ComponentName() { return "PointLight"; }

    virtual void SaveObject( nlohmann::json & aOutFile ) override;

    virtual void DrawEditorGUI() override;

private:

    PointLightData LightingData;

};