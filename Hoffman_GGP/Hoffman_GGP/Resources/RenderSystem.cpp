#include "RenderSystem.h"

#include "../Resources/SimpleShader.h"
#include "../Lighting/LightShaderDefs.h"


RenderSystem::RenderSystem()
{

}

RenderSystem::~RenderSystem()
{
    // Release references to any info
    DirLights.clear();
    PointLights.clear();
}

void RenderSystem::RenderFrame( SimpleVertexShader* aVertShader, SimplePixelShader* aPixShader )
{
    // For each mesh renderer ...
    SetLightData( aPixShader );
}

void RenderSystem::AddDirLight( DirectionalLight & aDirLight )
{
    DirLights.push_back( aDirLight );
}

void RenderSystem::AddPointLight( PointLightData & aPointLight )
{
    PointLights.push_back( aPointLight );
}

const std::vector<DirectionalLight>& RenderSystem::GetDirLights() const
{
    return DirLights;
}

const std::vector<PointLightData>& RenderSystem::GetPointLights() const
{
    return PointLights;
}

void RenderSystem::SetLightData( SimplePixelShader* aPixShader )
{
    // Send dir lights
    if ( DirLights.size() > 0 )
    {
        aPixShader->SetData( "DirLights", ( void* ) ( &DirLights[ 0 ] ), sizeof( DirectionalLight ) * MAX_DIR_LIGHTS );
    }
    aPixShader->SetInt( "DirLightCount", static_cast< int >( DirLights.size() ) );

    // Send point lights
    if ( PointLights.size() > 0 )
    {
        aPixShader->SetData( "PointLights", ( void* ) ( &PointLights[ 0 ] ), sizeof( PointLightData ) * MAX_POINT_LIGHTS );
    }
    aPixShader->SetInt( "PointLightCount", static_cast< int >( PointLights.size() ) );
}