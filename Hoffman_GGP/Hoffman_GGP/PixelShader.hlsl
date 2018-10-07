
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
    // Data type
    //  |
    //  |   Name          Semantic
    //  |    |                |
    //  v    v                v
    float4 position		: SV_POSITION;	// XYZW position (System Value Position)
    float2 uv           : TEXCOORD;
    float3 normal       : NORMAL;
    float3 worldPos		: POSITION; // The world position of this vertex
};



float4 CalculateLight( float3 norm, DirectionalLight aLight )
{
    float3 lightNormDir = normalize( -light.Direction );

    float NdotL = saturate( dot( norm, lightNormDir ) );

    return light.AmbientColor + light.DiffuseColor * NdotL;
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
    input.normal = normalize( input.normal );
    
    //float3 dirToCamera = normalize( CameraPosition - input.worldPos );


    float4 lightColor = 
        CalculateLight( input.normal, light ) +
        CalculateLight( input.normal, light_two );

    float3 normalFromMap = NormalTexture.Sample( BasicSampler, input.uv ).rgb * 2 - 1;


    // Texture Sample
    // Something is wrong when I return this
    float4 textureColor = DiffuseTexture.Sample( BasicSampler, input.uv );

    return float4( textureColor.rgb * lightColor, 1 );
}