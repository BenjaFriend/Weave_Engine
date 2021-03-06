#pragma once

#include "stdafx.h"
#include "Resources/ISaveable.h"

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
        ID3D11SamplerState* aSampler,
        FileName aMeshFileName );

    /** Destructor of the material */
    virtual ~Material();

    /// <summary>
    /// Save this material to the given asset file 
    /// </summary>
    /// <param name="aOutFile">File to send material data to</param>
    void SaveObject( nlohmann::json & aOutFile ) override;

    /// <summary>
    /// Sets any values to the shaders that are on this material
    /// and sends that data to the GPU
    /// </summary>
    virtual void SetShaderValues( const glm::mat4 & aWorld, const glm::mat4 & aView, const glm::mat4 & aProjection );

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

    FORCE_INLINE const std::string GetMeshFileName() const 
    {
        std::string name( MaterialFileName.begin(), MaterialFileName.end() );
        return name;
    }

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

    /** The file name that was used to create this material */
    FileName MaterialFileName;
};

