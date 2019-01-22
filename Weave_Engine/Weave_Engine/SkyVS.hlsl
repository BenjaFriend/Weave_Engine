
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
    float4 position		: SV_POSITION;
    float3 sampleDir	: TEXCOORD;
};


VertexToPixel main( VertexShaderInput input )
{
    // Set up output
    VertexToPixel output;

    // Adjust the view matrix to assume it has no translation
    matrix viewNoTranslation = view;
    viewNoTranslation._41 = 0;
    viewNoTranslation._42 = 0;
    viewNoTranslation._43 = 0;

    // Calculate output position
    matrix viewProj = mul( viewNoTranslation, projection );
    output.position = mul( float4( input.position, 1.0f ), viewProj );

    // Push the vertex out to the far clip plane (Z of 1)
    output.position.z = output.position.w; // Z gets divided by W automatically

                                           // Need to send the direction of the vertex down
                                           // to the pixel shader so we know which direction
                                           // to sample the cube map from
    output.sampleDir = input.position;

    return output;
}