#include "../stdafx.h"

#include "DirLight.h"

#include "../Resources/LightSystem.h"

#define AMBIENT_COLOR__SAVE_KEY      "AmbientColor"
#define DIFFUSE_COLOR__SAVE_KEY      "DiffuseColor"
#define INTENSITY_SAVE_KEY  "Intensity"
#define DIR_SAVE_KEY        "Direction"

COMPONENT_INIT( DirLight )

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

void DirLight::SaveComponentData( nlohmann::json const & comp_data )
{
    comp_data [ INTENSITY_SAVE_KEY ] = LightingData.Intensity;

    comp_data [ DIR_SAVE_KEY ] [ "X" ] = LightingData.Direction.x;
    comp_data [ DIR_SAVE_KEY ] [ "Y" ] = LightingData.Direction.y;
    comp_data [ DIR_SAVE_KEY ] [ "Z" ] = LightingData.Direction.z;

    comp_data [ DIFFUSE_COLOR__SAVE_KEY ] [ "X" ] = LightingData.DiffuseColor.x;
    comp_data [ DIFFUSE_COLOR__SAVE_KEY ] [ "Y" ] = LightingData.DiffuseColor.y;
    comp_data [ DIFFUSE_COLOR__SAVE_KEY ] [ "Z" ] = LightingData.DiffuseColor.z;

    comp_data [ AMBIENT_COLOR__SAVE_KEY ] [ "X" ] = LightingData.AmbientColor.x;
    comp_data [ AMBIENT_COLOR__SAVE_KEY ] [ "Y" ] = LightingData.AmbientColor.y;
    comp_data [ AMBIENT_COLOR__SAVE_KEY ] [ "Z" ] = LightingData.AmbientColor.z;
}

void DirLight::DrawEditorGUI()
{
    ImGui::Checkbox( "Is Enabled", &this->isEnabled );
    ImGui::ColorEdit3( "Ambient Color", ( float* ) &LightingData.AmbientColor );
    ImGui::ColorEdit3( "Diffuse Color", ( float* ) &LightingData.DiffuseColor );
    ImGui::DragFloat3( "Direction", ( float* ) &LightingData.Direction );
    ImGui::InputFloat( "Intensity", &LightingData.Intensity );
}
