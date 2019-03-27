#include "stdafx.h"

#include "DirLight.h"

#include "../Scenes/SceneManager.h"
#include "../Scenes/Scene.h"

#define AMBIENT_COLOR__SAVE_KEY      "AmbientColor"
#define DIFFUSE_COLOR__SAVE_KEY      "DiffuseColor"
#define INTENSITY_SAVE_KEY  "Intensity"
#define DIR_SAVE_KEY        "Direction"

COMPONENT_INIT( DirLight )

DirLight::DirLight()
{
    LightingData = {};
    
    SceneManagement::SceneManager::GetInstance()->GetActiveScene()->AddDirLight( this );
}

DirLight::DirLight( DirectionalLightData aLightData )
    : LightingData( aLightData )
{
    SceneManagement::SceneManager::GetInstance()->GetActiveScene()->AddDirLight( this );
}

DirLight::DirLight( nlohmann::json const & aInitData )
{
    LightingData = {};

    LightingData.Intensity = aInitData [ INTENSITY_SAVE_KEY ];

    LightingData.Direction.x = aInitData [ DIR_SAVE_KEY ] [ "X" ];
    LightingData.Direction.y = aInitData [ DIR_SAVE_KEY ] [ "Y" ];
    LightingData.Direction.z = aInitData [ DIR_SAVE_KEY ] [ "Z" ];

    LightingData.DiffuseColor.x = aInitData [ DIFFUSE_COLOR__SAVE_KEY ] [ "X" ];
    LightingData.DiffuseColor.y = aInitData [ DIFFUSE_COLOR__SAVE_KEY ] [ "Y" ];
    LightingData.DiffuseColor.z = aInitData [ DIFFUSE_COLOR__SAVE_KEY ] [ "Z" ];

    LightingData.AmbientColor.x = aInitData [ AMBIENT_COLOR__SAVE_KEY ] [ "X" ];
    LightingData.AmbientColor.y = aInitData [ AMBIENT_COLOR__SAVE_KEY ] [ "Y" ];
    LightingData.AmbientColor.z = aInitData [ AMBIENT_COLOR__SAVE_KEY ] [ "Z" ];

    SceneManagement::SceneManager::GetInstance()->GetActiveScene()->AddDirLight( this );
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

void DirLight::SaveComponentData( nlohmann::json & comp_data )
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
    if ( ImGui::Button( "Remove" ) )
    {
        OwningEntity->RemoveComponent<DirLight>();
    }
    ImGui::ColorEdit3( "Ambient Color", ( float* ) &LightingData.AmbientColor );
    ImGui::ColorEdit3( "Diffuse Color", ( float* ) &LightingData.DiffuseColor );
    ImGui::DragFloat3( "Direction", ( float* ) &LightingData.Direction );
    ImGui::InputFloat( "Intensity", &LightingData.Intensity );
}
