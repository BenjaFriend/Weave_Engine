#include "Material.h"
#include "SimpleShader.h"

Material::Material(SimpleVertexShader* aVertShader, SimplePixelShader* aPixShader, ID3D11ShaderResourceView * aSRV, ID3D11SamplerState* aSampler )
	: VertexShader (aVertShader), PixelShader (aPixShader), SRV (aSRV), Sampler ( aSampler )
{
	// TODO: Make the vertex and pixel shader pointers smart pointers or use 
	// some basic ref counting
}

Material::~Material()
{
	VertexShader = nullptr;
	PixelShader = nullptr;
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

ID3D11ShaderResourceView * Material::GetSRV() const
{
    return SRV;
}

ID3D11SamplerState * Material::GetSamplerState() const
{
    return Sampler;
}
