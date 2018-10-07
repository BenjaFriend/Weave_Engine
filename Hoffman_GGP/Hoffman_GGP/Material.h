#pragma once

#include "DebugSettings.h"


/////////////////////////////////////////////////
// Forward Declarations

class SimpleVertexShader;
class SimplePixelShader;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;

/// <summary>
///  Material class to define how the surface of a mesh looks when drawn
/// </summary>
/// <author>Ben Hoffman</author>
class Material
{
public:

    /// <summary>
    /// Constructor for this material
    /// </summary>
    /// <param name="aVertShader">Pointer to the vertex shader for this material</param>
    /// <param name="aPixShader">Pointer to the pixel shader for this material</param>
	Material(SimpleVertexShader* aVertShader, SimplePixelShader* aPixShader, ID3D11ShaderResourceView * aSRV, ID3D11SamplerState* aSampler );

	/** Destructor of the material */
	~Material();


	////////////////////////////////////////////////////
	// Accessors
	////////////////////////////////////////////////////

	/** Returns the vertex shader of this material */
	SimpleVertexShader* GetVertexShader() const;
	
	/** Returns the pixel shader of this material */
	SimplePixelShader* GetPixelShader() const;

    /** Returns this material's SRV */
    ID3D11ShaderResourceView* GetSRV() const;

    /** Return this material's sampler state */
    ID3D11SamplerState* GetSamplerState() const;

private: 

	/** Vertex shader of this material */
	SimpleVertexShader* VertexShader = nullptr;

	/** Pixel shader of this material */
	SimplePixelShader* PixelShader = nullptr;

    /** Shader Resource view of this Material. */
    ID3D11ShaderResourceView* SRV = nullptr;

    /** The sampler for this material's texture */
    ID3D11SamplerState* Sampler = nullptr;
};

