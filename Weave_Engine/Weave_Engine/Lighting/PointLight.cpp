#include "../stdafx.h"

#include "PointLight.h"

#include "../Resources/LightSystem.h"

PointLight::PointLight(
    LightSystem* aRendSys,
    glm::vec3 aColor,
    glm::vec3 aPos,
    float aIntensity,
    float aRange )
{
    LightingData = {};
    LightingData.Color = aColor;
    LightingData.Position = aPos;
    LightingData.Intensity = aIntensity;
    LightingData.Range = aRange;

    aRendSys->AddPointLight( this );
}

PointLight::~PointLight()
{

}

void PointLight::SetColor( glm::vec3 & aColor )
{
    LightingData.Color = aColor;
}

void PointLight::SetPosition( glm::vec3 aPos )
{
    LightingData.Position = aPos;
}

void PointLight::SetIntensity( float aVal )
{
    LightingData.Intensity = aVal;
}

void PointLight::SetRange( float aVal )
{
    LightingData.Range = aVal;
}

const PointLightData & PointLight::GetLightData() const
{
    return LightingData;
}

const bool PointLight::GetDrawRange() const
{
    return DrawRange;
}

void PointLight::SetDrawRange(bool aVal)
{
    DrawRange = aVal;
}

void PointLight::SaveObject( nlohmann::json & aOutFile )
{
}

void PointLight::DrawEditorGUI()
{
    ImGui::Checkbox( "Is Enabled", &this->isEnabled );
    ImGui::InputFloat3( "Pos Offset", ( float* ) &LightingData.Position );
    ImGui::InputFloat( "Range", &LightingData.Range );
    ImGui::InputFloat( "Intensity", &LightingData.Intensity );
    ImGui::ColorEdit3( "Color", ( float* ) &LightingData.Color );
    ImGui::Checkbox( "Draw Wireframe", &DrawRange );
}