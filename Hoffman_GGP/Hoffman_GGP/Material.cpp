#include "Material.h"
#include "SimpleShader.h"

Material::Material(SimpleVertexShader* aVertShader, SimplePixelShader* aPixShader) 
	: VertexShader (aVertShader), PixelShader (aPixShader)
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
