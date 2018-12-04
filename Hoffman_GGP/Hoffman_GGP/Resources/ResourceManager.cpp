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

const Mesh_ID ResourceManager::LoadMesh( char * aFileName )
{
    Meshes.push_back( new Mesh( currentDevice, aFileName ) );
    return Meshes.size() - 1;
}

Mesh * ResourceManager::GetMesh( const Mesh_ID aMeshID )
{
    assert( aMeshID >= 0 && aMeshID < Meshes.size() );

    return Meshes[ aMeshID ];
}

const SRV_ID ResourceManager::LoadSRV( ID3D11DeviceContext * aContext, wchar_t* aFileName )
{
    assert( aContext != nullptr );

    ID3D11ShaderResourceView* tempSRV = nullptr;

    HRESULT iResult = CreateWICTextureFromFile(
        currentDevice,
        aContext,
        aFileName,
        0,
        &tempSRV
    );

    // if success
    if ( iResult == S_OK )
    {
        SRViews.push_back( tempSRV );
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
        SRViews.push_back( tempSRV );

        return SRViews.size() - 1;
    }
    else
    {
        size_t i;
        constexpr size_t BUF_SIZE = 64;
        char fileName[ BUF_SIZE ];

        wcstombs_s( &i, fileName, BUF_SIZE, aFileName, BUF_SIZE );

        LOG_ERROR( "DDS SRV LOADING FAILURE! {}", fileName );

        return -1;
    }
}

ID3D11ShaderResourceView * ResourceManager::GetSRV( const SRV_ID aSrvID )
{
    assert( aSrvID >= 0 && aSrvID < SRViews.size() );

    return SRViews[ aSrvID ];
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

ID3D11SamplerState * ResourceManager::GetSampler( const size_t aID )
{
    assert( aID >= 0 && aID < Samplers.size() );

    return Samplers[ aID ];
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
        SRViews[ aDiffSrvID ],
        SRViews[ aNormSrvID ],
        SRViews[ aRoughnessSrvID ],
        SRViews[ aMetalSrvID ],
        Samplers[ aSamplerID ]
    );

    Materials.push_back( newMat );

    return Materials.size() - 1;
}

Material* ResourceManager::GetMaterial( const Material_ID aID )
{
    assert( aID >= 0 && aID < Materials.size() );

    return Materials[ aID ];
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

    // Unload shaders
    UnloadSRVs();

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
        ( *it )->Release();
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

inline const ID3D11Device * ResourceManager::GetCurrentDevice() const
{
    return currentDevice;
}
