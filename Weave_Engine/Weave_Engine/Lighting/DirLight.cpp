#include "../stdafx.h"

#include "DirLight.h"

#include "../Resources/LightSystem.h"


DirLight::DirLight( LightSystem* aRendSys, DirectionalLightData aLightData )
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
    nlohmann::json comp_data = nlohmann::json::object();

    comp_data [ "TestData" ] = "Hello this is fake data for the dir light";


    aOutFile [ ClassName() ] = comp_data;
}

void DirLight::DrawEditorGUI()
{
    ImGui::Checkbox( "Is Enabled", &this->isEnabled );
    ImGui::ColorEdit3( "Ambient Color", ( float* ) &LightingData.AmbientColor );
    ImGui::ColorEdit3( "Diffuse Color", ( float* ) &LightingData.DiffuseColor );
    ImGui::DragFloat3( "Direction", ( float* ) &LightingData.Direction );
    ImGui::InputFloat( "Intensity", &LightingData.Intensity );
}
