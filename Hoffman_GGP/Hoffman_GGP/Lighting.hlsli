// Include guard, the shader compiler doesn't support #pragma once
#ifndef _LIGHTING_HLSL
#define _LIGHTING_HLSL

#include "Lighting/LightShaderDefs.h"

// PBR Constants -----------------------------------------------

// The fresnel value for non-metals (dielectrics)
// Page 9: "F0 of nonmetals is now a constant 0.04"
// http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
// Also slide 65 of http://blog.selfshadow.com/publications/s2014-shading-course/hoffman/s2014_pbs_physics_math_slides.pdf
static const float F0_NON_METAL = 0.04f;

// Need a minimum roughness for when spec distribution function denominator goes to zero
static const float MIN_ROUGHNESS = 0.0000001f; // 6 zeros after decimal

static const float PI = 3.14159265359f;


// Basic lighting calculations -----------------------------------------------

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

// Fresnel term - Schlick approx.
// 
// v - View vector
// h - Half vector
// f0 - Value when l = n (full specular color)
//
// F(v,h,f0) = f0 + (1-f0)(1 - (v dot h))^5
float3 Fresnel( float3 v, float3 h, float3 f0 )
{
    // Pre-calculations
    float VdotH = saturate( dot( v, h ) );

    // Final value
    return f0 + ( 1 - f0 ) * pow( 1 - VdotH, 5 );
}

float SpecDistribution( float3 n, float3 h, float roughness )
{
    // Pre-calculations
    float NdotH = saturate( dot( n, h ) );
    float NdotH2 = NdotH * NdotH;
    float a = roughness * roughness;
    float a2 = max( a * a, MIN_ROUGHNESS ); // Applied after remap!

                                            // ((n dot h)^2 * (a^2 - 1) + 1)
    float denomToSquare = NdotH2 * ( a2 - 1 ) + 1;
    // Can go to zero if roughness is 0 and NdotH is 1

    // Final value
    return a2 / ( PI * denomToSquare * denomToSquare );
}

float GeometricShadowing( float3 n, float3 v, float3 h, float roughness )
{
    // End result of remapping:
    float k = pow( roughness + 1, 2 ) / 8.0f;
    float NdotV = saturate( dot( n, v ) );

    // Final value
    return NdotV / ( NdotV * ( 1 - k ) + k );
}


float3 MicrofacetBRDF( float3 n, float3 l, float3 v, float roughness, float metalness, float3 specColor )
{
    // Other vectors
    float3 h = normalize( v + l );

    // Grab various functions
    float D = SpecDistribution( n, h, roughness );
    float3 F = Fresnel( v, h, specColor ); // This ranges from F0_NON_METAL to actual specColor based on metalness
    float G = GeometricShadowing( n, v, h, roughness ) * GeometricShadowing( n, l, h, roughness );

    // Final formula
    // Denominator dot products partially canceled by G()!
    // See page 16: http://blog.selfshadow.com/publications/s2012-shading-course/hoffman/s2012_pbs_physics_math_notes.pdf
    return ( D * F * G ) / ( 4 * max( dot( n, v ), dot( n, l ) ) );
}

// Light Calculations -----------------------------------------------

float3 CalculateDirLight( float3 norm, DirectionalLight aLight )
{
    float3 lightNormDir = normalize( -aLight.Direction.rgb );

    float NdotL = saturate( dot( norm, lightNormDir ) );

    return float3( aLight.AmbientColor.rgb + aLight.DiffuseColor.rgb * NdotL );
}

// From Chris for now
float3 CalculatePointLight( PointLight light, float3 normal, float3 worldPos, float3 camPos, float roughness, float metalness, float3 surfaceColor, float3 specularColor )
{
    // Calc light direction
    float3 toLight = normalize( light.Position - worldPos );
    float3 toCam = normalize( camPos - worldPos );

    // Calculate the light amounts
    float atten = Attenuate( light, worldPos );
    float diff = saturate( dot( normal, toLight ) );
    float3 spec = MicrofacetBRDF( normal, toLight, toCam, roughness, metalness, specularColor );

    // Calculate diffuse with energy conservation
    // (Reflected light doesn't diffuse)
    float3 balancedDiff = diff * ( ( 1 - saturate( spec ) ) * ( 1 - metalness ) );

    // Combine
    return ( balancedDiff * surfaceColor + spec ) * atten * light.Intensity * light.Color;
}

// From chris
float3 DirLightPBR( DirectionalLight light, float3 normal, float3 worldPos, float3 camPos, float roughness, float metalness, float3 surfaceColor, float3 specularColor )
{
    // Get normalize direction to the light
    float3 toLight = normalize( -light.Direction );
    float3 toCam = normalize( camPos - worldPos );

    // Calculate the light amounts
    float diff = saturate( dot( normal, toLight ) );
    float3 spec = MicrofacetBRDF( normal, toLight, toCam, roughness, metalness, specularColor );

    // Calculate diffuse with energy conservation
    // (Reflected light doesn't get diffused)
    float3 balancedDiff = diff * ( ( 1 - saturate( spec ) ) * ( 1 - metalness ) );


    // Combine amount with 
    return float3( balancedDiff.rgb * surfaceColor.rgb + spec.rgb ) * light.Intensity * light.DiffuseColor.rgb;
}

#endif  // _LIGHTING_HLSL