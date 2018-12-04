#include "Material.h"
#include "../SimpleShader.h"

Material::Material(
    SimpleVertexShader* aVertShader,
    SimplePixelShader* aPixShader,
    ID3D11ShaderResourceView * aDiffuseSRV,
    ID3D11ShaderResourceView* aNormalSRV,
    ID3D11ShaderResourceView* aRoughnessSRV,
    ID3D11ShaderResourceView* aMetalSRV,
    ID3D11SamplerState* aSampler
)
    : VertexShader( aVertShader ),
    PixelShader( aPixShader ),
    DiffuseSRV( aDiffuseSRV ),
    NormalSRV( aNormalSRV ),
    RoughnessSRV( aRoughnessSRV ),
    MetalSRV( aMetalSRV ),
    Sampler( aSampler )
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
    MetalSRV = nullptr;
    RoughnessSRV = nullptr;
    Sampler = nullptr;
}

void Material::SaveObject( nlohmann::json & aOutFile )
{
    // Out the file names of any shaders that we have
    LOG_WARN( "Implement Material class save!!" );    
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

void Material::SetShininess( float aVal )
{
    Shininiess = aVal;
}

const float Material::GetShininess() const
{
    return Shininiess;
}

ID3D11ShaderResourceView* Material::GetMetalSRV() const
{
    return MetalSRV;
}

ID3D11ShaderResourceView* Material::GetRoughnessSRV() const
{
    return RoughnessSRV;
}