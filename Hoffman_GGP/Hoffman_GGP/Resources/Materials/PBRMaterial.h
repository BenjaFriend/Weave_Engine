#pragma once


#include "Material.h"


/// <summary>
/// A physically based rendering material
/// </summary>
/// <author>Ben Hoffman</akuthor>
class PBRMaterial :
    public Material
{
public:

    PBRMaterial( SimpleVertexShader* aVertShader,
        SimplePixelShader* aPixShader,
        ID3D11ShaderResourceView* aDiffuseSRV,
        ID3D11ShaderResourceView* aNormalSRV,
        ID3D11SamplerState* aSampler );

    virtual ~PBRMaterial() override;

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

private:

    /// <summary>
    /// How shiny this material is. Default is 0.5.
    /// Should stay between 0 and 1.
    /// </summary>
    float Shininiess = 0.5f;

    ID3D11ShaderResourceView * RoughnessSRV;

    ID3D11ShaderResourceView* MetalSRV;

};

