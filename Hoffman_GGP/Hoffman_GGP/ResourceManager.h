#pragma once

#include "DebugSettings.h"


#include <vector>
#include "d3d11.h"	// ID3D11Buffer
#include "WICTextureLoader.h"

/////////////////////////////////////////////////
// Forward Declarations
class Mesh;
class Material;

/// <summary>
/// Loading and unloading all graphics resources like SRV's, meshes, and 
/// Materials
/// </summary>
/// <author>Ben Hoffman</author>
class ResourceManager
{
public:

    /// <summary>
    /// Initialize the resource manager with any DX11 resources necessary
    /// </summary>
    /// <returns>Static instance of the resource manager</returns>
    static ResourceManager* Initalize( ID3D11Device* aDevice );

    /// <summary>
    /// Gets a reference to the current resource manager
    /// </summary>
    /// <returns>Static instance of the resource manager</returns>
    static ResourceManager* GetInstance();

    /// <summary>
    /// Release the current Resource Manager and all resources
    /// </summary>
    static void ReleaseInstance();

    /// <summary>
    /// Loads in a mesh with the given file name
    /// </summary>
    /// <param name="aFileName"></param>
    /// <returns>An ID for the current mesh file to be used elsewhere</returns>
    const UINT LoadMesh( char* aFileName );



private:

    ResourceManager( ID3D11Device* aDevice );

    ~ResourceManager();

    void UnloadMeshes();

    void UnloadMaterials();

    static ResourceManager* Instance;

    std::vector<Mesh*> Meshes;

    std::vector<Material*> Materials;

    ID3D11Device* currentDevice;

};

