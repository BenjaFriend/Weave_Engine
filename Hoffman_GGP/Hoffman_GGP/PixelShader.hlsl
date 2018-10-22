
#include "Lighting.hlsli"


cbuffer externalData : register( b0 )
{    
    // Array of dir light data
    DirectionalLight DirLights[ MAX_DIR_LIGHTS ];
    int LightCount;

    DirectionalLight DirLightTest;  // A test field right now to see how I am passing in data

    float3 CameraPosition; // Needed for specular (reflection) calculation
};

struct VertexToPixel
{
    float4 position		: SV_POSITION;	// XYZW position (System Value Position)
    float2 uv           : TEXCOORD;
    float3 normal       : NORMAL;
    float3 tangent		: TANGENT;
    float3 worldPos		: POSITION; // The world position of this vertex
};

// Define globals for the texture samples
Texture2D DiffuseTexture : register( t0 );
Texture2D NormalTexture : register( t1 );
SamplerState BasicSampler : register( s0 );

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
    // Re-normalize interpolated data
    input.normal = normalize( input.normal );
    
    // Something could be wrong with this, it is all black
    input.tangent = normalize( input.tangent );
    //return float4( input.tangent.rgb, 1 );        
    
    // Sample normal map
    float3 normalFromMap = NormalTexture.Sample( BasicSampler, input.uv ).rgb * 2 - 1;

    // Sample diffuse texture
    float4 textureColor = DiffuseTexture.Sample( BasicSampler, input.uv );

    // Create my TBN matrix to convert from tangent-space to world-space
    float3 N = input.normal;
    float3 T = normalize( input.tangent - N * dot( input.tangent, N ) ); // Ensure tangent is 90 degrees from normal
    float3 B = cross( T, N );
    float3x3 TBN = float3x3( T, B, N );

    input.normal = normalize( mul( normalFromMap, TBN ) );

    // Calculate lighting --------------------------------
    float3 lightColor = float3( 0, 0, 0 );

    // Not working, just stays at 0 for it all
    for ( int i = 0; i < LightCount; ++i )
    {
        lightColor += CalculateDirLight( input.normal, DirLights[ i ] );
    }

    // See this will work
    //lightColor = CalculateDirLight( input.normal, DirLightTest );


    return float4 ( lightColor.rgb * textureColor.rgb, 1 );
}