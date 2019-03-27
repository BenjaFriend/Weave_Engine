#include "stdafx.h"

#include "PointLight.h"
#include "../Scenes/SceneManager.h"
#include "../Scenes/Scene.h"

#define COLOR_SAVE_KEY      "Color"
#define INTENSITY_SAVE_KEY  "Intensity"
#define RANGE_SAVE_KEY      "Range"
#define POS_SAVE_KEY        "Pos"

COMPONENT_INIT( PointLight )

PointLight::PointLight()
{
    LightingData = {};
    PosOffset = glm::vec3( 0.f );
    SceneManagement::SceneManager::GetInstance()->GetActiveScene()->AddPointLight( this );
}

PointLight::PointLight(
    glm::vec3 aColor,
    glm::vec3 aPosOffset,
    float aIntensity,
    float aRange )
{
    LightingData = {};
    LightingData.Color = aColor;
    LightingData.Intensity = aIntensity;
    LightingData.Range = aRange;

    PosOffset = aPosOffset;

    SceneManagement::SceneManager::GetInstance()->GetActiveScene()->AddPointLight( this );
}

PointLight::PointLight( PointLightData aData, const glm::vec3 & aPosOffset )
    : PointLight( aData.Color, aPosOffset, aData.Intensity, aData.Range )
{
}

PointLight::PointLight( nlohmann::json const & aInitData )
{
    LightingData = {};
    LightingData.Intensity = aInitData [ INTENSITY_SAVE_KEY ];
    LightingData.Range = aInitData [ RANGE_SAVE_KEY ];

    LightingData.Color.r = aInitData [ COLOR_SAVE_KEY ] [ "R" ];
    LightingData.Color.g = aInitData [ COLOR_SAVE_KEY ] [ "G" ];
    LightingData.Color.b = aInitData [ COLOR_SAVE_KEY ] [ "B" ];

    PosOffset.x = aInitData [ POS_SAVE_KEY ] [ "X" ];
    PosOffset.y = aInitData [ POS_SAVE_KEY ] [ "Y" ];
    PosOffset.z = aInitData [ POS_SAVE_KEY ] [ "Z" ];

    SceneManagement::SceneManager::GetInstance()->GetActiveScene()->AddPointLight( this );
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

const PointLightData & PointLight::GetLightData() 
{
    Transform* transform = this->GetEntity()->GetTransform();
    const glm::vec3 pos = ( transform->GetPosition() + PosOffset );
    LightingData.Position.x = pos.x;
    LightingData.Position.y = pos.y;
    LightingData.Position.z = pos.z;
    return LightingData;
}

const bool PointLight::GetDrawRange() const
{
    return DrawRange;
}

void PointLight::SetDrawRange( bool aVal )
{
    DrawRange = aVal;
}

void PointLight::SaveComponentData( nlohmann::json & comp_data )
{
    comp_data [ INTENSITY_SAVE_KEY ] = LightingData.Intensity;
    comp_data [ RANGE_SAVE_KEY ] = LightingData.Range;

    comp_data [ COLOR_SAVE_KEY ] [ "R" ] = LightingData.Color.r;
    comp_data [ COLOR_SAVE_KEY ] [ "G" ] = LightingData.Color.g;
    comp_data [ COLOR_SAVE_KEY ] [ "B" ] = LightingData.Color.b;

    comp_data [ POS_SAVE_KEY ] [ "X" ] = PosOffset.x;
    comp_data [ POS_SAVE_KEY ] [ "Y" ] = PosOffset.y;
    comp_data [ POS_SAVE_KEY ] [ "Z" ] = PosOffset.z;
}

void PointLight::DrawEditorGUI()
{
    ImGui::Checkbox( "Is Enabled", &this->isEnabled );
    ImGui::InputFloat3( "Pos Offset", ( float* ) &PosOffset );
    ImGui::InputFloat( "Range", &LightingData.Range );
    ImGui::InputFloat( "Intensity", &LightingData.Intensity );
    ImGui::ColorEdit3( "Color", ( float* ) &LightingData.Color );
    ImGui::Checkbox( "Draw Wireframe", &DrawRange );
}