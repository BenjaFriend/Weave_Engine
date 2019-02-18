#include "../stdafx.h"

#include "PointLight.h"

#include "../Resources/LightSystem.h"

#define COLOR_SAVE_KEY      "Color"
#define INTENSITY_SAVE_KEY  "Intensity"
#define RANGE_SAVE_KEY      "Range"
#define POS_SAVE_KEY        "Pos"

COMPONENT_INIT( PointLight )

PointLight::PointLight(
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

    SceneManagement::SceneManager::GetInstance()->GetActiveScene()->AddPointLight( this );
}

PointLight::PointLight( nlohmann::json const & aInitData )
{
    LOG_WARN( "Point light load from scene file not yet implemetned!" );
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

void PointLight::SaveComponentData( nlohmann::json & comp_data )
{
    comp_data [ INTENSITY_SAVE_KEY ] = LightingData.Intensity;
    comp_data [ RANGE_SAVE_KEY ] = LightingData.Range;

    comp_data [ COLOR_SAVE_KEY ] [ "R" ] = LightingData.Color.r;
    comp_data [ COLOR_SAVE_KEY ] [ "G" ] = LightingData.Color.g;
    comp_data [ COLOR_SAVE_KEY ] [ "B"] = LightingData.Color.b;

    comp_data [ POS_SAVE_KEY ] [ "X" ] = LightingData.Position.x;
    comp_data [ POS_SAVE_KEY ] [ "Y" ] = LightingData.Position.y;
    comp_data [ POS_SAVE_KEY ] [ "Z" ] = LightingData.Position.z;
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