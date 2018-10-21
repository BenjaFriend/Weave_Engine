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

float4 CalculateDirLight( float3 norm, DirectionalLight aLight )
{
    float3 lightNormDir = normalize( -aLight.Direction );

    float NdotL = saturate( dot( norm, lightNormDir ) );

    return aLight.AmbientColor + aLight.DiffuseColor * NdotL;
}

#endif  // _LIGHTING_HLSL