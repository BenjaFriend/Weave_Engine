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

void RenderSystem::AddPointLight( PointLightData * aPointLight )
{
    PointLights.push_back( aPointLight );
}

const std::vector<DirectionalLight>& RenderSystem::GetDirLights() const
{
    return DirLights;
}

const std::vector<PointLightData*> & RenderSystem::GetPointLights() const
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
        // I need a pointer to these bois that is continuous in 
        // memory so that I can send it to the GPU
        PointLightData newArr[ MAX_POINT_LIGHTS ];
        for ( size_t i = 0; i < PointLights.size(); ++i )
        {
            memcpy( ( void* ) ( newArr + i ), ( void* ) PointLights[ i ], sizeof( PointLightData ) );
        }

        aPixShader->SetData( "PointLights", ( void* ) ( &newArr[ 0 ] ), sizeof( PointLightData ) * MAX_POINT_LIGHTS );
    }
    aPixShader->SetInt( "PointLightCount", static_cast< int >( PointLights.size() ) );
}