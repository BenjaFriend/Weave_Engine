#include "../stdafx.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Materials/Material.h"

// Singleton definitions
ResourceManager* ResourceManager::Instance = nullptr;
ID3D11Device* ResourceManager::currentDevice = nullptr;
ID3D11DeviceContext* ResourceManager::currentContext = nullptr;

using namespace  DirectX;

ResourceManager * ResourceManager::Initalize( ID3D11Device* aDevice, ID3D11DeviceContext* aContext )
{
    assert( Instance == nullptr && aDevice != nullptr );

    Instance = new ResourceManager( aDevice, aContext );

    return Instance;
}

ResourceManager * ResourceManager::GetInstance()
{
    assert( Instance != nullptr );

    return Instance;
}

// Private constructor
ResourceManager::ResourceManager( ID3D11Device* aDevice, ID3D11DeviceContext* aContext )
{ 
    currentDevice = aDevice;
    currentContext = aContext;
}

// Private destructor
ResourceManager::~ResourceManager()
{
    UnloadMeshes();

    UnloadMaterials();

    UnloadSRVs();

    UnloadShaders();

    UnloadRasterizers();

    currentDevice = nullptr;
    currentContext = nullptr;
}

void ResourceManager::ReleaseInstance()
{
    if ( Instance != nullptr )
    {
        delete Instance;
        Instance = nullptr;
    }
}

Mesh* ResourceManager::LoadMesh( const FileName & aFileName )
{
    // Ensure that this mesh isn't already loaded
    if ( Meshes.find( aFileName ) != Meshes.end() )
    {
        return Meshes [ aFileName ];
    }

    Mesh* newMesh = new Mesh( currentDevice, aFileName );

    Meshes [ aFileName ] = newMesh;
    
    return newMesh;
}

Mesh * ResourceManager::GetMesh( const FileName & aFileName )
{
    if ( Meshes.find( aFileName ) != Meshes.end() )
    {
        return Meshes [ aFileName ];
    }

    return nullptr;
}

ID3D11ShaderResourceView* ResourceManager::LoadSRV( const FileName & aFileName )
{
    assert( currentContext != nullptr );
    // If this SRV already exists, than just return that 
    if ( SRViews.find( aFileName ) != SRViews.end() )
    {
        return SRViews [ aFileName ];
    }

    // Load this texture
    ID3D11ShaderResourceView* tempSRV = nullptr;

    HRESULT iResult = CreateWICTextureFromFile(
        currentDevice,
        currentContext,
        aFileName.c_str(),
        0,
        &tempSRV
    );

    // if success
    if ( iResult == S_OK )
    {
        SRViews [ aFileName ] = tempSRV;
        return tempSRV;
    }
    else
    {
        LOG_ERROR( "SRV LOADING FAILURE!" );
        return nullptr;
    }
}

ID3D11ShaderResourceView* ResourceManager::LoadSRV_DDS( const FileName & aFileName )
{
    assert( currentContext != nullptr );
    // If this SRV already exists, than just return that 
    if ( SRViews.find( aFileName ) != SRViews.end() )
    {
        return SRViews [ aFileName ];
    }

    ID3D11ShaderResourceView* tempSRV = nullptr;

    HRESULT iResult = CreateDDSTextureFromFile(
        currentDevice,
        currentContext,
        aFileName.c_str(),
        0,
        &tempSRV
    );

    // if success
    if ( iResult == S_OK )
    {
        SRViews [ aFileName ] = tempSRV;

        return tempSRV;
    }
    else
    {
        size_t i;
        constexpr size_t BUF_SIZE = 64;
        char fileName [ BUF_SIZE ];

        wcstombs_s( &i, fileName, BUF_SIZE, aFileName.c_str(), BUF_SIZE );

        LOG_ERROR( "DDS SRV LOADING FAILURE! {}", fileName );

        return nullptr;
    }
}

ID3D11RasterizerState * ResourceManager::LoadRasterizerState( const std::string & aName, D3D11_RASTERIZER_DESC & rsState )
{
    // Check if this exists already
    if ( RastStates.find( aName ) != RastStates.end() ) return RastStates [ aName ];

    ID3D11RasterizerState* newRS = nullptr;
    HRESULT iRes = currentDevice->CreateRasterizerState( &rsState, &newRS );
    if ( iRes == S_OK )
    {
        RastStates [ aName ] = newRS;
        LOG_TRACE( "Loaded {} Rasterizer state!", aName );
        return newRS;
    }
    else
    {
        LOG_ERROR( "Failed to load rasterizer state!" );
        return nullptr;
    }
}

ID3D11RasterizerState * ResourceManager::GetRasterizerState( const std::string & aName )
{
    if ( RastStates.find( aName ) != RastStates.end() ) return RastStates [ aName ];

    return nullptr;
}

ID3D11ShaderResourceView * ResourceManager::GetSRV( const SRV_ID & aSrvID )
{
    return SRViews [ aSrvID ];
}

const Sampler_ID ResourceManager::AddSampler( D3D11_SAMPLER_DESC & aSamplerDesc )
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

ID3D11SamplerState * ResourceManager::GetSampler( const Sampler_ID aID )
{
    assert( aID >= 0 && aID < Samplers.size() );

    return Samplers [ aID ];
}

Material* ResourceManager::LoadMaterial(
    const Material_ID aName,
    SimpleVertexShader* aVertexShader,
    SimplePixelShader* aPixelShader,
    const SRV_ID aDiffSrvID,
    const SRV_ID aNormSrvID,
    const SRV_ID aRoughnessSrvID,
    const SRV_ID aMetalSrvID,
    const Sampler_ID aSamplerID
)
{
    // If this material already exists, than just return that 
    if ( Materials.find( aName ) != Materials.end() )
    {
        return Materials [ aName ];
    }

    // #TODO: Only create this material if does not yet exist
    Material* newMat = new Material(
        aVertexShader,
        aPixelShader,
        LoadSRV( aDiffSrvID ), 
        LoadSRV ( aNormSrvID ),
        LoadSRV ( aRoughnessSrvID ),
        LoadSRV ( aMetalSrvID ),
        Samplers [ aSamplerID ]
    );

    Materials [ aName ] = newMat;
    LOG_TRACE( "Loaded new material: {}", aName );
    return newMat;
}

Material* ResourceManager::GetMaterial( const Material_ID aID )
{
    if ( Materials.find( aID ) != Materials.end() )
    {
        return Materials [ aID ];
    }
    return nullptr;
}

void ResourceManager::UnloadMeshes()
{
    // Delete each mesh that has been added
    for ( auto itr = Meshes.begin(); itr != Meshes.end(); ++itr )
    {
        if ( itr->second != nullptr )
            delete ( itr->second );
    }

    Meshes.clear();
    LOG_TRACE( "Unloaded Meshes!" );
}

void ResourceManager::UnloadMaterials()
{
    // Delete each material that has been added
    for ( auto itr = Materials.begin(); itr != Materials.end(); ++itr )
    {
        if ( itr->second != nullptr )
            delete ( itr->second );
    }
    Materials.clear();

    LOG_TRACE( "Unloaded Materials!" );
}

void ResourceManager::UnloadSRVs()
{
    auto itr = SRViews.begin();
    for ( ; itr != SRViews.end(); ++itr )
    {
        if ( itr->second != nullptr )
            itr->second->Release();
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

void ResourceManager::UnloadRasterizers()
{
    auto itr = RastStates.begin();
    for ( ; itr != RastStates.end(); ++itr )
    {
        if ( itr->second != nullptr )
            itr->second->Release();
    }
    RastStates.clear();
    LOG_TRACE( "Unloaded RastStates!" );
}

const ID3D11Device * ResourceManager::GetCurrentDevice() const
{
    return currentDevice;
}

void ResourceManager::SetCurrentDevice( ID3D11Device * aDev )
{
    assert( aDev != nullptr );
    currentDevice = aDev;
}

const ID3D11DeviceContext * ResourceManager::GetCurrentContext() const
{
    return currentContext;
}

void ResourceManager::SetCurrentContext( ID3D11DeviceContext * aContext )
{
    assert( aContext != nullptr );
    currentContext = aContext;
}
