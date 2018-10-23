
cbuffer externalData : register( b0 )
{
    matrix view;
    matrix projection;
};


struct VertexShaderInput
{
    float3 position		: POSITION;     // XYZ position
    float2 uv           : TEXCOORD;
    float3 normal       : NORMAL;
    float3 tangent		: TANGENT;
};

struct VertexToPixel
{
    float4 position		: SV_POSITION;	// XYZW position (System Value Position)
    float2 sampleDir    : TEXCOORD;
};


VertexToPixel main( VertexShaderInput input )
{
    // Set up output struct
    VertexToPixel output;

    
    //  Adjust the view matrix to assume it has no tranlsation
    matrix viewNoTranslation = view;
    viewNoTranslation._41 = 0;
    viewNoTranslation._42 = 0;
    viewNoTranslation._43 = 0;

    // Calculate output position
    matrix viewProj = mul( viewNoTranslation, projection );
    output.position = mul( float4( input.position, 1.0f ), viewProj );

    // Push the vertex out to the far cliip plane (z of 1)
    output.position.z = output.position.w;  // Z gets divided by W automatically, so it's 1

    // rip
    output.sampleDir = input.position;


    return output;
}