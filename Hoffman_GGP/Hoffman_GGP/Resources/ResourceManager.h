#pragma once

#include "../stdafx.h"

#if defined( _WIN32 ) || defined ( _WIN64 )

#include "d3d11.h"	// ID3D11Buffer
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

#else


#endif

#include <vector>
#include <type_traits>

#include "SimpleShader.h"
#include "Mesh.h"

/////////////////////////////////////////////////
// Forward Declarations
class Material;
class SimpleVertexShader;
class SimplePixelShader;
class ISimpleShader;

using Mesh_ID       = size_t;
using Material_ID   = std::string;
using SRV_ID        = FileName;
using Sampler_ID    = size_t;

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
    /// <param name="aDevice">Device to load assets with</param>
    /// <param name="aContext">Context to load assets with</param>
    /// <returns>Static instance of the resource manager</returns>
    static ResourceManager* Initalize(
        ID3D11Device* aDevice, 
        ID3D11DeviceContext* aContext
    );

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
    /// <param name="aFileName">The filename of the mesh</param>
    /// <returns>An ID for the current mesh file to be used elsewhere</returns>
    Mesh* LoadMesh( const FileName & aFileName );

    /// <summary>
    /// Get a pointer to a mesh with it's ID
    /// </summary>
    /// <param name="aMeshID">ID for the given mesh</param>
    /// <returns>mesh pointer to the given object</returns>
    Mesh* GetMesh( const FileName & aMeshID );

    /// <summary>
    /// Load in an SRV with the given context
    /// </summary>
    /// <param name="aContext">Device context to use</param>
    /// <param name="aFileName">File path of the assets</param>
    /// <returns>ID of the SRV to use for any later use</returns>
    ID3D11ShaderResourceView* LoadSRV( const FileName & aFileName );

    /// <summary>
    /// Load in a DDS SRV (cube map) 
    /// </summary>
    /// <param name="aFileName">File where this txture can be found</param>
    /// <returns>Pointer to the texture after it is loaded</returns>
    ID3D11ShaderResourceView* LoadSRV_DDS( const FileName & aFileName );

    template<class T>
    T* LoadShader(
        std::wstring aFileName,
        ID3D11Device* aDevice = currentDevice,
        ID3D11DeviceContext* aContext = currentContext
    )
    {
        static_assert(
            std::is_base_of<ISimpleShader, T>::value,
            "T is not derived from ISimpleShader."
            );

        // Check if this shader is already loaded, if so , then return it
        auto got = Shaders.find( aFileName );
        if ( got != Shaders.end() )
        {
            return reinterpret_cast< T* >( got->second );
        }

        // Create a new shader
        ISimpleShader* shader = new T( currentDevice, currentContext);
        shader->LoadShaderFile( aFileName.c_str() );

        // Add to the map of shaders
        Shaders.emplace( aFileName, shader );

#if defined( _DEBUG ) || defined ( DEBUG )
        const size_t size = 64;
        char buffer [ size ];

        size_t ret;
        wcstombs_s( &ret, buffer, aFileName.c_str(), size );
        if ( ret == size - 1 ) buffer [ size - 1 ] = '\0';
        LOG_TRACE( "Shader Loaded: {}", buffer );
#endif

        return reinterpret_cast< T* >( shader );
    }

    /// <summary>
    /// Gets an SRV bsaed on the given ID
    /// </summary>
    /// <param name="aSrvID"></param>
    /// <returns></returns>
    ID3D11ShaderResourceView* GetSRV( const SRV_ID & aSrvID );

    /// <summary>
    /// Adds a new sampler with the given device to the current target device
    /// </summary>
    /// <param name="samplerDesc">Options for the given sampler desc</param>
    /// <returns>ID of the sampler that was created</returns>
    const Sampler_ID AddSampler( D3D11_SAMPLER_DESC & samplerDesc );

    /// <summary>
    /// Gets a pointer to the sampler state with the given ID
    /// </summary>
    /// <param name="aID">ID of the sampler you wish to access</param>
    /// <returns>pointer to the sampler state with the given ID</returns>
    ID3D11SamplerState* GetSampler( const Sampler_ID aID );

    Material* LoadMaterial(
        const Material_ID aName,
        SimpleVertexShader* aVertexShader,
        SimplePixelShader* aPixelShader,
        const SRV_ID aDiffSrvID,
        const SRV_ID aNormSrvID,
        const SRV_ID aRoughnessSrvID,
        const SRV_ID aMetalSrvID,
        const Sampler_ID aSamplerID
    );

    /// <summary>
    /// Get a pointer to the given material with this ID
    /// </summary>
    /// <param name="aID">ID of the mesh you are trying to access</param>
    /// <returns>Pointer to the given mesh</returns>
    Material* GetMaterial( const Material_ID aID );

    /// <summary>
    /// Gets a pointer to the current target device
    /// </summary>
    /// <returns>Pointer to the current device</returns>
    const ID3D11Device* GetCurrentDevice() const;

    void SetCurrentDevice( ID3D11Device* aDev );

    const ID3D11DeviceContext* GetCurrentContext() const;

    void SetCurrentContext( ID3D11DeviceContext* aContext );

private:

    ResourceManager( ID3D11Device* aDevice, ID3D11DeviceContext* aContext );

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

    /// <summary>
    /// Deletes all currently loaded shaders
    /// </summary>
    void UnloadShaders();

    /** Static instance of the resource manager */
    static ResourceManager* Instance;

    /** Loaded shader files */
    std::unordered_map<std::wstring, ISimpleShader*> Shaders;

    /** The currently loaded meshes */
    std::unordered_map<FileName, Mesh*> Meshes;

    /** Loaded materials */
    std::unordered_map<std::string, Material*> Materials;

    /// <summary>
    /// The currently loaded Shader Resource Views
    /// </summary>
    std::unordered_map<FileName, ID3D11ShaderResourceView*> SRViews;

    /// <summary>
    /// Currently loaded samplers to use
    /// </summary>
    std::vector<ID3D11SamplerState*> Samplers;

    /// <summary>
    /// The current graphics target device
    /// </summary>
    static ID3D11Device* currentDevice;

    /// <summary>
    /// Keep track of the current context so that we can easily load in
    /// SRVs and other items that may need it
    /// </summary>
    static ID3D11DeviceContext* currentContext;
};