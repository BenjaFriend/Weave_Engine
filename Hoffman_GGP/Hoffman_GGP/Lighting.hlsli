// Include guard
#ifndef _LIGHTING_HLSL
#define _LIGHTING_HLSL

#define MAX_DIR_LIGHTS 64

struct DirectionalLight
{
    float4 AmbientColor;
    float4 DiffuseColor;
    float3 Direction;
};

float3 CalculateDirLight( float3 norm, DirectionalLight aLight )
{
    float3 lightNormDir = normalize( -aLight.Direction.rgb );

    float NdotL = saturate( dot( norm, lightNormDir ) );

    return float3( aLight.AmbientColor.rgb + aLight.DiffuseColor.rgb * NdotL );
}

#endif  // _LIGHTING_HLSL