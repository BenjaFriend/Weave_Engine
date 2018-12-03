#pragma once

#include <vector>


/////////////////////////////////////////////////
// Forward Declarations
class SimplePixelShader;
class SimpleVertexShader;
class PointLight;
struct PointLightData;
struct DirectionalLight;

/// <summary>
/// A render system will track different types of meshes or other drawable
/// resources, and render them on it's draw function. 
/// </summary>
class RenderSystem
{
public:

    RenderSystem();

    ~RenderSystem();

    /// <summary>
    /// Render all meshes/lights in this system
    /// </summary>
    /// <param name="aVertShader">The vert shader to use</param>
    /// <param name="aPixShader"></param>
    void RenderFrame( SimpleVertexShader* aVertShader, SimplePixelShader* aPixShader );

    /// <summary>
    /// Add a dir light to this system
    /// </summary>
    /// <param name="aDirLight">The dir light to be added</param>
    void AddDirLight( DirectionalLight & aDirLight );

    /// <summary>
    /// Add a point light to this render system
    /// </summary>
    /// <param name="aPointLight">The point light to be added</param>
    void AddPointLight( PointLight * aPointLight );

    const std::vector<DirectionalLight> & GetDirLights() const;

    const std::vector<PointLight*> & GetPointLights() const;

private:

    /// <summary>
    /// Send all lighting information to the given shaders
    /// </summary>
    /// <param name="aPixShader">Pixel shader to send lighting info to</param>
    void SetLightData( SimplePixelShader* aPixShader );

    std::vector<DirectionalLight> DirLights;

    std::vector<PointLight*> PointLights;

};