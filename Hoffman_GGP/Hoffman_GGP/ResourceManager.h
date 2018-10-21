#pragma once

#include "DebugSettings.h"


#include <vector>
#include "d3d11.h"	// ID3D11Buffer
#include "WICTextureLoader.h"

/////////////////////////////////////////////////
// Forward Declarations
class Mesh;
class Material;
class SimpleVertexShader;
class SimplePixelShader;

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

    /// <summary>
    /// Get a pointer to a mesh with it's ID
    /// </summary>
    /// <param name="aMeshID">ID for the given mesh</param>
    /// <returns>mesh pointer to the given object</returns>
    Mesh* GetMesh( const UINT aMeshID );

    /// <summary>
    /// Load in an SRV with the given context
    /// </summary>
    /// <param name="aContext">Device context to use</param>
    /// <param name="aFileName">File path of the assets</param>
    /// <returns>ID of the SRV to use for any later use</returns>
    const UINT LoadSRV( ID3D11DeviceContext* aContext, wchar_t* aFileName );

    /// <summary>
    /// Gets an SRV bsaed on the given ID
    /// </summary>
    /// <param name="aSrvID"></param>
    /// <returns></returns>
    ID3D11ShaderResourceView* GetSRV( const UINT aSrvID );

    /// <summary>
    /// Adds a new sampler with the given device to the current target device
    /// </summary>
    /// <param name="samplerDesc">Options for the given sampler desc</param>
    /// <returns>ID of the sampler that was created</returns>
    const UINT AddSampler( D3D11_SAMPLER_DESC & samplerDesc );

    /// <summary>
    /// Gets a pointer to the sampler state with the given ID
    /// </summary>
    /// <param name="aID">ID of the sampler you wish to access</param>
    /// <returns>pointer to the sampler state with the given ID</returns>
    ID3D11SamplerState* GetSampler( const UINT aID );

    /// <summary>
    /// 
    /// </summary>
    /// <param name="aVertexShader"></param>
    /// <param name="aPixelShader"></param>
    /// <param name="aDiffSrvID"></param>
    /// <param name="aNormSrvID"></param>
    /// <param name="aSamplerID"></param>
    /// <returns></returns>
    const UINT LoadMaterial( SimpleVertexShader* aVertexShader, SimplePixelShader* aPixelShader, const UINT aDiffSrvID, const UINT aNormSrvID, const UINT aSamplerID );

    /// <summary>
    /// Get a pointer to the given material with this ID
    /// </summary>
    /// <param name="aID">ID of the mesh you are trying to access</param>
    /// <returns>Pointer to hte given mesh</returns>
    Material* GetMaterial( const UINT aID );

    /// <summary>
    /// Gets a pointer to the current target device
    /// </summary>
    /// <returns>Pointer to the current device</returns>
    inline const ID3D11Device* GetCurrentDevice() const;

private:

    ResourceManager( ID3D11Device* aDevice );

    ~ResourceManager();

    /// <summary>
    /// Delete all current meshes that have been created
    /// </summary>
    void UnloadMeshes();

    /// <summary>
    /// Delete all current materials that are loaded in
    /// </summary>
    void UnloadMaterials();

    /// <summary>
    /// Unload all current Shader resource Views
    /// </summary>
    void UnloadSRVs();

    static ResourceManager* Instance;

    std::vector<Mesh*> Meshes;

    std::vector<Material*> Materials;

    /// <summary>
    /// The currently loaded Shader Resource Views
    /// </summary>
    std::vector<ID3D11ShaderResourceView*> SRViews;

    /// <summary>
    /// Currently loaded samplers to use
    /// </summary>
    std::vector<ID3D11SamplerState*> Samplers;

    /// <summary>
    /// The current graphics target device
    /// </summary>
    ID3D11Device* currentDevice;

};

