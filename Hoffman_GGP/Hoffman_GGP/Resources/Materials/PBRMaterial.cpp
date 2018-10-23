#include "PBRMaterial.h"



PBRMaterial::PBRMaterial( SimpleVertexShader * aVertShader, SimplePixelShader * aPixShader, ID3D11ShaderResourceView * aDiffuseSRV, ID3D11ShaderResourceView * aNormalSRV, ID3D11SamplerState * aSampler )
    : Material( aVertShader, aPixShader, aDiffuseSRV, aNormalSRV, aSampler )
{
    DEBUG_PRINT( "PBR Mat CTOR BOIII" );

}

PBRMaterial::~PBRMaterial()
{

    DEBUG_PRINT( "PBR Mat DTOR BOIII" );

    MetalSRV = nullptr;
    RoughnessSRV = nullptr;

    Material::~Material();

}


////////////////////////////////////////////////////
// Accessors
////////////////////////////////////////////////////

void PBRMaterial::SetShininess( float aVal )
{
    Shininiess = aVal;
}

const float PBRMaterial::GetShininess() const
{
    return Shininiess;
}

ID3D11ShaderResourceView* PBRMaterial::GetMetalSRV() const
{
    return MetalSRV;
}

ID3D11ShaderResourceView* PBRMaterial::GetRoughnessSRV() const
{
    return RoughnessSRV;
}