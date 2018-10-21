#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"

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

const UINT ResourceManager::LoadMesh( char * aFileName )
{
    Meshes.push_back( new Mesh( currentDevice, aFileName ) );
    return static_cast<UINT>( Meshes.size() - 1 );
}

Mesh * ResourceManager::GetMesh( const UINT aMeshID )
{
    assert( aMeshID >= 0 && aMeshID < Meshes.size() );

    return Meshes[ aMeshID ];
}

const UINT ResourceManager::LoadSRV( ID3D11DeviceContext * aContext, wchar_t* aFileName )
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

        return static_cast<UINT> ( SRViews.size() - 1 );
    }
    else
    {
        DEBUG_PRINT( "SRV LOADING FAILURE!" );

        return -1;
    }
}

ID3D11ShaderResourceView * ResourceManager::GetSRV( const UINT aSrvID )
{
    assert( aSrvID >= 0 && aSrvID < SRViews.size() );

    return SRViews[ aSrvID ];
}

const UINT ResourceManager::AddSampler( D3D11_SAMPLER_DESC & aSamplerDesc )
{
    ID3D11SamplerState* NewSamplerState = nullptr;
    HRESULT iResult = currentDevice->CreateSamplerState( &aSamplerDesc, &NewSamplerState );

    if ( iResult == S_OK )
    {
        Samplers.push_back( NewSamplerState );
        return static_cast<UINT>( Samplers.size() - 1 );
    }
    else
    {
        return -1;
    }
}

ID3D11SamplerState * ResourceManager::GetSampler( const UINT aID )
{
    assert( aID >= 0 && aID < Samplers.size() );

    return Samplers[ aID ];
}

const UINT ResourceManager::LoadMaterial( SimpleVertexShader* aVertexShader, SimplePixelShader* aPixelShader, const UINT aDiffSrvID, const UINT aNormSrvID, const UINT aSamplerID )
{
    Material* newMat = new Material(
        aVertexShader,
        aPixelShader,
        SRViews[ aDiffSrvID ],
        SRViews[ aNormSrvID ],
        Samplers[ aSamplerID ]
    );

    Materials.push_back( newMat );

    return static_cast<UINT>( Materials.size() - 1 );
}

Material* ResourceManager::GetMaterial( const UINT aID )
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
    DEBUG_PRINT( "Unloaded Meshes!" );

}

void ResourceManager::UnloadMaterials()
{
    // Delete each material that has been added
    for ( auto it = Materials.begin(); it != Materials.end(); ++it )
    {
        delete ( *it );
    }

    Materials.clear();
    DEBUG_PRINT( "Unloaded Materials!" );

}

void ResourceManager::UnloadSRVs()
{
    // Release each DX11 resource that was loaded here
    for ( auto it = SRViews.begin(); it != SRViews.end(); ++it )
    {
        ( *it )->Release();
    }

    SRViews.clear();
    DEBUG_PRINT( "Unloaded SRVs!" );

    // Release each DX11 resource that was loaded here
    for ( auto it = Samplers.begin(); it != Samplers.end(); ++it )
    {
        ( *it )->Release();
    }

    Samplers.clear();
    DEBUG_PRINT( "Unloaded Samplers!" );

}

inline const ID3D11Device * ResourceManager::GetCurrentDevice() const
{
    return currentDevice;
}
