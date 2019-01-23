cbuffer externalData : register( b0 )
{
    float3 Color;
}


float4 main() : SV_TARGET
{
    return float4( Color, 1 );
}