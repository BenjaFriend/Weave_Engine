#pragma once

#include "../stdafx.h"

#include <DirectXMath.h>

#include "LightShaderDefs.h"
#include "../ECS/Component.h"

class PointLight : public ECS::Component<PointLight>
{
public:

    PointLight( DirectX::XMFLOAT3 aColor,
        DirectX::XMFLOAT3 aPos, 
        float aIntensity = 1.f, 
        float aRange = 5.f
    );

    ~PointLight();

    const PointLightData& GetLightData() const;

    virtual const char* ComponentName() { return "PointLight"; }

    virtual void SaveObject( nlohmann::json & aOutFile ) override;

private:

    PointLightData LightingData;

};