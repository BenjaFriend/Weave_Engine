#include "../stdafx.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Materials/Material.h"

// Singleton definitions
ResourceManager* ResourceManager::Instance = nullptr;
using namespace  DirectX;

ResourceManager * ResourceManager::Initalize( ID3D11Device* aDevice )
{
    assert( Instance == nullptr && aDevice != nullptr );

    Instance = new ResourceManager( aDevice );

    return Instance;
}

ResourceManager * ResourceManager::GetInstance()
{
    assert( Instance != nullptr );

    return Instance;
}

void ResourceManager::ReleaseInstance()
{
    if ( Instance != nullptr )
    {
        delete Instance;
        Instance = nullptr;
    }
}

const std::tuple<Mesh_ID, Mesh*> ResourceManager::LoadMesh( FileName aFileName )
{
    // Ensure that this mesh isn't already loaded
    for ( size_t i = 0; i < Meshes.size(); ++i )
    {
        if ( Meshes [ i ]->fileName == aFileName  )
        {
            return std::make_tuple( i, Meshes [ i ]->mesh );
        }
    }

    //const size_t size = 64;
    //char fileNameBuf [ 128 ];
    //size_t ret;
    //wcstombs_s( &ret, fileNameBuf, aFileName, size );

    Mesh* newMesh = new Mesh( currentDevice, aFileName );
    LoadedMesh* newLoadedMesh = new LoadedMesh( aFileName, newMesh );

    Meshes.push_back( newLoadedMesh );
    return std::make_tuple( Meshes.size() - 1, newMesh );
}

Mesh * ResourceManager::GetMesh( const Mesh_ID aMeshID )
{
    assert( aMeshID >= 0 && aMeshID < Meshes.size() );

    return Meshes [ aMeshID ]->mesh;
}

const SRV_ID ResourceManager::LoadSRV( ID3D11DeviceContext * aContext, FileName aFileName )
{
    assert( aContext != nullptr );

    // Ensure that this isn't already loaded in
    // Expensive, but only happens at program start
    // and stops texture from being loaded in more than once
    for ( size_t i = 0; i < SRViews.size(); ++i )
    {
        if ( SRViews [ i ]->fileName == aFileName )
            return i;
    }

    // Load this texture
    ID3D11ShaderResourceView* tempSRV = nullptr;

    HRESULT iResult = CreateWICTextureFromFile(
        currentDevice,
        aContext,
        aFileName.c_str(),
        0,
        &tempSRV
    );

    // if success
    if ( iResult == S_OK )
    {
        LoadedSRV* newSrv = new LoadedSRV( aFileName, tempSRV );

        SRViews.push_back( newSrv );
        return static_cast< size_t > ( SRViews.size() - 1 );
    }
    else
    {
        LOG_ERROR( "SRV LOADING FAILURE!" );
        return -1;
    }
}

const SRV_ID ResourceManager::LoadSRV_DDS( ID3D11DeviceContext * aContext, wchar_t * aFileName )
{
    assert( aContext != nullptr );

    ID3D11ShaderResourceView* tempSRV = nullptr;

    HRESULT iResult = CreateDDSTextureFromFile(
        currentDevice,
        aContext,
        aFileName,
        0,
        &tempSRV
    );

    // if success
    if ( iResult == S_OK )
    {
        LoadedSRV* newSrv = new LoadedSRV( aFileName, tempSRV );

        SRViews.push_back( newSrv );

        return SRViews.size() - 1;
    }
    else
    {
        size_t i;
        constexpr size_t BUF_SIZE = 64;
        char fileName [ BUF_SIZE ];

        wcstombs_s( &i, fileName, BUF_SIZE, aFileName, BUF_SIZE );

        LOG_ERROR( "DDS SRV LOADING FAILURE! {}", fileName );

        return -1;
    }
}

ID3D11ShaderResourceView * ResourceManager::GetSRV( const SRV_ID aSrvID )
{
    assert( aSrvID >= 0 && aSrvID < SRViews.size() );

    return SRViews [ aSrvID ]->srv;
}

const SRV_ID ResourceManager::AddSampler( D3D11_SAMPLER_DESC & aSamplerDesc )
{
    ID3D11SamplerState* NewSamplerState = nullptr;
    HRESULT iResult = currentDevice->CreateSamplerState( &aSamplerDesc, &NewSamplerState );

    if ( iResult == S_OK )
    {
        Samplers.push_back( NewSamplerState );
        return Samplers.size() - 1;
    }
    else
    {
        return -1;
    }
}

ID3D11SamplerState * ResourceManager::GetSampler( const SRV_ID aID )
{
    assert( aID >= 0 && aID < Samplers.size() );

    return Samplers [ aID ];
}

const Material_ID ResourceManager::LoadMaterial(
    SimpleVertexShader* aVertexShader,
    SimplePixelShader* aPixelShader,
    const SRV_ID aDiffSrvID,
    const SRV_ID aNormSrvID,
    const SRV_ID aRoughnessSrvID,
    const SRV_ID aMetalSrvID,
    const Sampler_ID aSamplerID
)
{
    Material* newMat = new Material(
        aVertexShader,
        aPixelShader,
        SRViews [ aDiffSrvID ]->srv,
        SRViews [ aNormSrvID ]->srv,
        SRViews [ aRoughnessSrvID ]->srv,
        SRViews [ aMetalSrvID ]->srv,
        Samplers [ aSamplerID ]
    );

    Materials.push_back( newMat );

    return Materials.size() - 1;
}

Material* ResourceManager::GetMaterial( const Material_ID aID )
{
    assert( aID >= 0 && aID < Materials.size() );

    return Materials [ aID ];
}

// Private constructor
ResourceManager::ResourceManager( ID3D11Device* aDevice )
    : currentDevice( aDevice )
{

}

// Private destructor
ResourceManager::~ResourceManager()
{
    UnloadMeshes();

    UnloadMaterials();

    UnloadSRVs();

    UnloadShaders();

    currentDevice = nullptr;
}

void ResourceManager::UnloadMeshes()
{
    // Delete each mesh that has been added
    for ( auto it = Meshes.begin(); it != Meshes.end(); ++it )
    {
        delete ( *it );
    }

    Meshes.clear();
    LOG_TRACE( "Unloaded Meshes!" );

}

void ResourceManager::UnloadMaterials()
{
    // Delete each material that has been added
    for ( auto it = Materials.begin(); it != Materials.end(); ++it )
    {
        delete ( *it );
    }

    Materials.clear();
    LOG_TRACE( "Unloaded Materials!" );

}

void ResourceManager::UnloadSRVs()
{
    // Release each DX11 resource that was loaded here
    for ( auto it = SRViews.begin(); it != SRViews.end(); ++it )
    {
        delete *( it );
    }

    SRViews.clear();
    LOG_TRACE( "Unloaded SRVs!" );

    // Release each DX11 resource that was loaded here
    for ( auto it = Samplers.begin(); it != Samplers.end(); ++it )
    {
        ( *it )->Release();
    }

    Samplers.clear();
    LOG_TRACE( "Unloaded Samplers!" );

}

void ResourceManager::UnloadShaders()
{
    auto itr = Shaders.begin();
    for ( ; itr != Shaders.end(); ++itr )
    {
        if ( itr->second != nullptr )
            delete ( itr->second );
    }
    Shaders.clear();
}

inline const ID3D11Device * ResourceManager::GetCurrentDevice() const
{
    return currentDevice;
}
