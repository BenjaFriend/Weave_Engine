#include "PointLight.h"

PointLight::PointLight( DirectX::XMFLOAT3 aColor,
    DirectX::XMFLOAT3 aPos,
    float aIntensity = 1.f,
    float aRange = 5.f )
{
    LightingData = {};
    LightingData.Color = aColor;
    LightingData.Position = aPos;
    LightingData.Intensity = aIntensity;
    LightingData.Range = aRange;
}

PointLight::~PointLight()
{

}

const PointLightData & PointLight::GetLightData() const
{
    return LightingData;
}

void PointLight::SaveObject( nlohmann::json & aOutFile )
{

}
