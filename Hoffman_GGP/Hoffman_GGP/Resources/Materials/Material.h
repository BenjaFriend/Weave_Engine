#pragma once

#include "../../stdafx.h"
#include "../ISaveable.h"

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
    : public ISaveable
{
public:

    /// <summary>
    /// Constructor for this material
    /// </summary>
    /// <param name="aVertShader">Pointer to the vertex shader for this material</param>
    /// <param name="aPixShader">Pointer to the pixel shader for this material</param>
    Material(
        SimpleVertexShader* aVertShader,
        SimplePixelShader* aPixShader,
        ID3D11ShaderResourceView* aDiffuseSRV,
        ID3D11ShaderResourceView* aNormalSRV,
        ID3D11ShaderResourceView* aRoughnessSRV,
        ID3D11ShaderResourceView* aMetalSRV,
        ID3D11SamplerState* aSampler );

    /** Destructor of the material */
    virtual ~Material();

    /// <summary>
    /// Save this material to the given asset file 
    /// </summary>
    /// <param name="aOutFile">File to send material data to</param>
    void SaveObject( nlohmann::json & aOutFile ) override;


    ////////////////////////////////////////////////////
    // Accessors
    ////////////////////////////////////////////////////

    /// <summary>
    /// Set the shininess of this material
    /// </summary>
    /// <param name="aVal">New shiny value</param>
    void SetShininess( float aVal );

    /// <summary>
    /// How shiny this object is
    /// </summary>
    /// <returns>The shininess of this object</returns>
    const float GetShininess() const;

    /** Returns the vertex shader of this material */
    SimpleVertexShader* GetVertexShader() const;

    /** Returns the pixel shader of this material */
    SimplePixelShader* GetPixelShader() const;

    /** Returns this material's diffuse texture SRV */
    ID3D11ShaderResourceView* GetDiffuseSRV() const;

    /** Returns this material's normal map SRV */
    ID3D11ShaderResourceView* GetNormalSRV() const;

    /// <summary>
    /// Get a reference to this materials roughness shader resource view
    /// </summary>
    /// <returns>a reference to this roughness SRV</returns>
    ID3D11ShaderResourceView* GetRoughnessSRV() const;

    /// <summary>
    /// Get a reference to this materials metal shader resource view
    /// </summary>
    /// <returns>reference to this materials metal SRV</returns>
    ID3D11ShaderResourceView* GetMetalSRV() const;

    /** Return this material's sampler state */
    ID3D11SamplerState* GetSamplerState() const;

protected:

    /// <summary>
    /// How shiny this material is. Default is 0.5.
    /// Should stay between 0 and 1.
    /// </summary>
    float Shininiess = 0.5f;

    /** Vertex shader of this material */
    SimpleVertexShader * VertexShader = nullptr;

    /** Pixel shader of this material */
    SimplePixelShader* PixelShader = nullptr;

    /** Shader Resource view of this Material. */
    ID3D11ShaderResourceView* DiffuseSRV = nullptr;

    /** Shader Resource view of this Material. */
    ID3D11ShaderResourceView* NormalSRV = nullptr;

    /** Shader resource view of this material */
    ID3D11ShaderResourceView * RoughnessSRV = nullptr;

    /** Shader resource view of this material */
    ID3D11ShaderResourceView* MetalSRV = nullptr;

    /** The sampler for this material's texture */
    ID3D11SamplerState* Sampler = nullptr;
};

