#include "PointLight.h"

#include "../Resources/RenderSystem.h"

PointLight::PointLight(
    RenderSystem* aRendSys,
    DirectX::XMFLOAT3 aColor,
    DirectX::XMFLOAT3 aPos,
    float aIntensity,
    float aRange )
{
    LightingData = {};
    LightingData.Color = aColor;
    LightingData.Position = aPos;
    LightingData.Intensity = aIntensity;
    LightingData.Range = aRange;

    aRendSys->AddPointLight( &LightingData );
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

void PointLight::DrawEditorGUI()
{
    ImGui::InputFloat( "Range", &LightingData.Range );
    ImGui::InputFloat( "Intensity", &LightingData.Intensity );
    ImGui::ColorEdit3( "Color", ( float* ) &LightingData.Color );
}
