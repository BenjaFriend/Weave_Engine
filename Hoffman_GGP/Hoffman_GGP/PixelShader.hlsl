
struct DirectionalLight
{
    float4 AmbientColor;
    float4 DiffuseColor;
    float3 Direction;
};

cbuffer externalData : register( b0 )
{
    DirectionalLight light;
    DirectionalLight light_two;


    float3 CameraPosition; // Needed for specular (reflection) calculation
};

// Define globals for the texture samples
Texture2D DiffuseTexture : register( t0 );
Texture2D NormalTexture : register( t1 );
SamplerState BasicSampler : register( s0 );

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
    float4 position		: SV_POSITION;	// XYZW position (System Value Position)
    float2 uv           : TEXCOORD;
    float3 normal       : NORMAL;
    float3 tangent		: TANGENT;
    float3 worldPos		: POSITION; // The world position of this vertex
};


float4 CalculateLight( float3 norm, DirectionalLight aLight )
{
    float3 lightNormDir = normalize( -aLight.Direction );

    float NdotL = saturate( dot( norm, lightNormDir ) );

    return aLight.AmbientColor + aLight.DiffuseColor * NdotL;
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
    // Re-normalize interpolated data
    input.normal = normalize( input.normal );
    input.tangent = normalize( input.tangent );

    float3 normalFromMap = NormalTexture.Sample( BasicSampler, input.uv ).rgb * 2 - 1;

    // Create my TBN matrix to convert from tangent-space to world-space
    float3 N = input.normal;
    float3 T = normalize( input.tangent - N * dot( input.tangent, N ) ); // Ensure tangent is 90 degrees from normal
    float3 B = cross( T, N );
    float3x3 TBN = float3x3( T, B, N );

    input.normal = normalize( mul( normalFromMap, TBN ) );

    //float dirLightAmount = saturate( dot( input.normal, -normalize( light.Direction ) ) );

    float4 lightColor = CalculateLight( input.normal, light ) + CalculateLight( input.normal, light_two );
    
    // Texture Sample
    float4 textureColor = DiffuseTexture.Sample( BasicSampler, input.uv );

    return float4 ( lightColor * textureColor.rgb , 1 );
    //return float4 ( light.DiffuseColor * dirLightAmount * textureColor.rgb , 1 );

}