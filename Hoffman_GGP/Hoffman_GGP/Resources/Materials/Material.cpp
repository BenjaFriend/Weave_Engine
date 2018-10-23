#include "Material.h"
#include "../SimpleShader.h"

Material::Material( SimpleVertexShader* aVertShader, SimplePixelShader* aPixShader, ID3D11ShaderResourceView * aDiffuseSRV, ID3D11ShaderResourceView* aNormalSRV, ID3D11SamplerState* aSampler )
    : VertexShader( aVertShader ), PixelShader( aPixShader ), DiffuseSRV( aDiffuseSRV ), NormalSRV( aNormalSRV ), Sampler( aSampler )
{
    // TODO: Make the vertex and pixel shader pointers smart pointers or use 
    // some basic ref counting
}

Material::~Material()
{
    // This material did not create these resource, so just remove the references
    // to them
    VertexShader = nullptr;
    PixelShader = nullptr;
    DiffuseSRV = nullptr;
    NormalSRV = nullptr;
    Sampler = nullptr;
    DEBUG_PRINT( "Material class dtor!" );
}

////////////////////////////////////////////////////
// Accessors
////////////////////////////////////////////////////

SimpleVertexShader * Material::GetVertexShader() const
{
    return VertexShader;
}

SimplePixelShader * Material::GetPixelShader() const
{
    return PixelShader;
}

ID3D11ShaderResourceView * Material::GetDiffuseSRV() const
{
    return DiffuseSRV;
}

ID3D11ShaderResourceView * Material::GetNormalSRV() const
{
    return NormalSRV;
}

ID3D11SamplerState * Material::GetSamplerState() const
{
    return Sampler;
}
