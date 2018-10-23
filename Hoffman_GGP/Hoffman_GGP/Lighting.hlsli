// Include guard
#ifndef _LIGHTING_HLSL
#define _LIGHTING_HLSL

#include "Lighting/LightShaderDefs.h"

//#define MAX_DIR_LIGHTS 64

struct DirectionalLight
{
    float4 AmbientColor;
    float4 DiffuseColor;
    float3 Direction;
    float Padding;      // This has to be here because of the way that
                        // Simple Shader works
};


struct PointLight
{
    float3 Color;           // 12 bytes
    float Range;            // 16 bytes

    float3 Position;        // 28 bytes
    float Intensity;        // 32 bytes
};

// Range-based attenuation function // From Chris Cascioli
float Attenuate( PointLight light, float3 worldPos )
{
    float dist = distance( light.Position, worldPos );

    // Ranged-based attenuation
    float att = saturate( 1.0f - ( dist * dist / ( light.Range * light.Range ) ) );

    // Soft falloff
    return att * att;
}

// Blinn-Phong (specular) BRDF  // From Chris Cascioli
float SpecularBlinnPhong( float3 normal, float3 dirToLight, float3 toCamera, float shininess )
{
    // Calculate halfway vector
    float3 halfwayVector = normalize( dirToLight + toCamera );

    // Compare halflway vector and normal and raise to a power
    return shininess == 0 ? 0.0f : pow( max( dot( halfwayVector, normal ), 0 ), shininess );
}



float3 CalculateDirLight( float3 norm, DirectionalLight aLight )
{
    float3 lightNormDir = normalize( -aLight.Direction.rgb );

    float NdotL = saturate( dot( norm, lightNormDir ) );

    return float3( aLight.AmbientColor.rgb + aLight.DiffuseColor.rgb * NdotL );
}

// From Chris for now
float3 CalculatePointLight( PointLight light, float3 normal, float3 worldPos, float3 camPos, float shininess, float roughness, float3 surfaceColor )
{
    // Calc light direction
    float3 toLight = normalize( light.Position - worldPos );
    float3 toCam = normalize( camPos - worldPos );

    // Calculate the light amounts
    float atten = Attenuate( light, worldPos );
    float diff = saturate( dot( normal, toLight ) );
    float spec = SpecularBlinnPhong( normal, toLight, toCam, shininess ) * ( 1.0f - roughness );

    // Combine
    return ( diff * surfaceColor + spec ) * atten * light.Intensity * light.Color;
}

#endif  // _LIGHTING_HLSL