
struct VertexToPixel
{
    float4 position		: SV_POSITION;	// XYZW position (System Value Position)
    float2 sampleDir    : TEXCOORD;
};

// Define globals for the texture samples
Texture2D SkyTexture        : register( t0 );
SamplerState BasicSampler   : register( s0 );

// Entry point for this pixel shader
float4 main( VertexToPixel input ) : SV_TARGET
{
    return SkyTexture.Sample( BasicSampler, input.sampleDir );
}