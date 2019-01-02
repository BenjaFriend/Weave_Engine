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
using SRV_ID        = size_t;
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
    /// <param name="aFileName">The filename of the mesh</param>
    /// <returns>An ID for the current mesh file to be used elsewhere</returns>
    const std::tuple<Mesh_ID, Mesh*> LoadMesh( FileName aFileName );

    /// <summary>
    /// Get a pointer to a mesh with it's ID
    /// </summary>
    /// <param name="aMeshID">ID for the given mesh</param>
    /// <returns>mesh pointer to the given object</returns>
    Mesh* GetMesh( const Mesh_ID aMeshID );

    /// <summary>
    /// Load in an SRV with the given context
    /// </summary>
    /// <param name="aContext">Device context to use</param>
    /// <param name="aFileName">File path of the assets</param>
    /// <returns>ID of the SRV to use for any later use</returns>
    const SRV_ID LoadSRV( ID3D11DeviceContext* aContext, FileName aFileName );

    const SRV_ID LoadSRV_DDS( ID3D11DeviceContext* aContext, wchar_t* aFileName );

    template<class T>
    T* LoadShader(
        ID3D11Device* aDevice,
        ID3D11DeviceContext* aContext,
        std::wstring aFileName
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
        ISimpleShader* shader = new T( aDevice, aContext );
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
    ID3D11ShaderResourceView* GetSRV( const SRV_ID aSrvID );

    /// <summary>
    /// Adds a new sampler with the given device to the current target device
    /// </summary>
    /// <param name="samplerDesc">Options for the given sampler desc</param>
    /// <returns>ID of the sampler that was created</returns>
    const SRV_ID AddSampler( D3D11_SAMPLER_DESC & samplerDesc );

    /// <summary>
    /// Gets a pointer to the sampler state with the given ID
    /// </summary>
    /// <param name="aID">ID of the sampler you wish to access</param>
    /// <returns>pointer to the sampler state with the given ID</returns>
    ID3D11SamplerState* GetSampler( const SRV_ID aID );

    /// <summary>
    /// 
    /// </summary>
    /// <param name="aVertexShader"></param>
    /// <param name="aPixelShader"></param>
    /// <param name="aDiffSrvID"></param>
    /// <param name="aNormSrvID"></param>
    /// <param name="aSamplerID"></param>
    /// <returns></returns>
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

    /// <summary>
    /// Deletes all currently loaded shaders
    /// </summary>
    void UnloadShaders();

    static ResourceManager* Instance;

    /// <summary>
    /// Keep track of a loaded SRV and it's file name
    /// so that we can check if it has already been loaded 
    /// in or not and keep 0(1) access over the vector in 
    /// the drawing method
    /// </summary>
    struct LoadedSRV
    {
        LoadedSRV( FileName aName, ID3D11ShaderResourceView* aSrv )
            : fileName( aName ), srv( aSrv )
        {
        }

        ~LoadedSRV()
        {
            if ( srv != nullptr )
            {
                srv->Release();
                srv = nullptr;
            }
        }

        FileName fileName = nullptr;
        ID3D11ShaderResourceView* srv = nullptr;

    };

    struct LoadedMesh
    {
        LoadedMesh( FileName aName, Mesh* aMesh )
            : fileName( aName ), mesh( aMesh )
        {
        }

        ~LoadedMesh()
        {
            if ( mesh != nullptr )
            {
                delete mesh;
                mesh = nullptr;
            }
        }

        FileName fileName = nullptr;
        Mesh* mesh = nullptr;
    };


    std::unordered_map<std::wstring, ISimpleShader*> Shaders;

    std::vector<LoadedMesh*> Meshes;

    //std::vector<Material*> Materials;

    std::unordered_map<std::string, Material*> Materials;

    /// <summary>
    /// The currently loaded Shader Resource Views
    /// </summary>
    std::vector<LoadedSRV*> SRViews;

    /// <summary>
    /// Currently loaded samplers to use
    /// </summary>
    std::vector<ID3D11SamplerState*> Samplers;

    /// <summary>
    /// The current graphics target device
    /// </summary>
    ID3D11Device* currentDevice;

};