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
    /// Initialize the 
    /// </summary>
    /// <returns></returns>
    static ResourceManager* Initalize();

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
    /// <returns>Pointer to the mesh instance</returns>
    static Mesh* LoadMesh( const char* aFileName );


private:

    ResourceManager();

    ~ResourceManager();

    void UnloadMeshes();

    void UnloadMaterials();

    static ResourceManager* Instance;

    static std::vector<Mesh*> Meshes;

    static std::vector<Material*> Materials;

    

};

