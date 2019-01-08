#pragma once

#include <vector>

/////////////////////////////////////////////////
// Forward Declarations
class SimplePixelShader;
class SimpleVertexShader;
class PointLight;
class DirLight;
struct PointLightData;
struct DirectionalLightData;

/// <summary>
/// A light system will keep track of any lights that are in the scene
/// and be able to set whatever data is necessary on a set of shaders
/// </summary>
/// <author>Ben Hoffman</author>
class LightSystem
{
public:

    LightSystem();

    ~LightSystem();

    /// <summary>
    /// Set data about this 
    /// </summary>
    /// <param name="aVertShader">The vert shader to use</param>
    /// <param name="aPixShader"></param>
    void SetShaderInfo( SimpleVertexShader* aVertShader, SimplePixelShader* aPixShader );

    /// <summary>
    /// Add a directional light to this system
    /// </summary>
    /// <param name="aDirLight">The dir light to be added</param>
    void AddDirLight( DirLight * aDirLight );

    /// <summary>
    /// Add a point light to this render system
    /// </summary>
    /// <param name="aPointLight">The point light to be added</param>
    void AddPointLight( PointLight * aPointLight );

    const std::vector<DirLight*> & GetDirLights() const;

    const std::vector<PointLight*> & GetPointLights() const;

private:

    /// <summary>
    /// Send all lighting information to the given shaders
    /// </summary>
    /// <param name="aPixShader">Pixel shader to send lighting info to</param>
    void SetLightData( SimplePixelShader* aPixShader );

    std::vector<DirLight*> DirLights;

    std::vector<PointLight*> PointLights;

};