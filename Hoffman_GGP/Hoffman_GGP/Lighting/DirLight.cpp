#include "DirLight.h"

#include "../Resources/RenderSystem.h"


DirLight::DirLight( RenderSystem* aRendSys, DirectionalLightData aLightData )
    : LightingData( aLightData )
{
    aRendSys->AddDirLight( this );
}

DirLight::~DirLight()
{
}

const DirectionalLightData & DirLight::GetLightData() const
{
    return LightingData;
}

void DirLight::SetLightData( DirectionalLightData aLightData )
{
    LightingData = aLightData;
}

void DirLight::SaveObject( nlohmann::json & aOutFile )
{
    // #Implement
}

void DirLight::DrawEditorGUI()
{
    ImGui::Checkbox( "Is Enabled", &this->isEnabled );
    ImGui::ColorEdit3( "Ambient Color", ( float* ) &LightingData.AmbientColor );
    ImGui::ColorEdit3( "Diffuse Color", ( float* ) &LightingData.DiffuseColor );
    ImGui::DragFloat3( "Direction", ( float* ) &LightingData.Direction );
    ImGui::InputFloat( "Intensity", &LightingData.Intensity );
}
