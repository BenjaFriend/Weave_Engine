#include "RenderSystem.h"

#include "../Resources/SimpleShader.h"
#include "../Lighting/LightShaderDefs.h"
#include "../Lighting/PointLight.h"
#include "../Lighting/DirLight.h"

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

void RenderSystem::AddDirLight( DirLight * aDirLight )
{
    DirLights.push_back( aDirLight );
}

void RenderSystem::AddPointLight( PointLight * aPointLight )
{
    PointLights.push_back( aPointLight );
}

const std::vector<DirLight*> & RenderSystem::GetDirLights() const
{
    return DirLights;
}

const std::vector<PointLight*> & RenderSystem::GetPointLights() const
{
    return PointLights;
}

void RenderSystem::SetLightData( SimplePixelShader* aPixShader )
{
    // Send dir lights -------------------------------
    size_t dirLightCount = DirLights.size();

    if ( DirLights.size() > 0 )
    {
        // There needs to be a raw array to the dir light data that is continuous in memory
        // in order for the GPU to be able to read it
        DirectionalLightData dirlightData[ MAX_DIR_LIGHTS ];

        for ( size_t i = 0; i < DirLights.size(); ++i )
        {
            // Skip if this component is disabled
            if ( !DirLights[ i ]->IsEnabled() )
            {
                --dirLightCount;
                continue;
            }
            // Copy the lighting data to the raw array of lighting data
            memcpy(
                ( void* ) ( dirlightData + i ),
                ( ( void* ) ( &DirLights[ i ]->GetLightData() ) ),
                sizeof( DirectionalLightData )
            );
        }
        // Send data to the GPU
        aPixShader->SetData( "DirLights", ( void* ) ( &dirlightData[ 0 ] ), sizeof( DirectionalLightData ) * MAX_DIR_LIGHTS );
    }
    aPixShader->SetInt( "DirLightCount", static_cast< int >( dirLightCount ) );

    // Send point lights -------------------------------
    size_t pointLightCount = PointLights.size();

    if ( PointLights.size() > 0 )
    {
        PointLightData pointLightData[ MAX_POINT_LIGHTS ];

        for ( size_t i = 0; i < PointLights.size(); ++i )
        {
            // Skip if disabled
            if ( !PointLights[ i ]->IsEnabled() )
            {
                --pointLightCount;
                continue;
            }
            // Copy the point light data to the raw array 
            memcpy(
                ( void* ) ( pointLightData + i ),
                ( ( void* ) ( &PointLights[ i ]->GetLightData() ) ),
                sizeof( PointLightData )
            );
        }
        // Send the raw data to the GPU
        aPixShader->SetData( "PointLights", ( void* ) ( &pointLightData[ 0 ] ), sizeof( PointLightData ) * MAX_POINT_LIGHTS );
    }
    aPixShader->SetInt( "PointLightCount", static_cast< int >( pointLightCount ) );
}