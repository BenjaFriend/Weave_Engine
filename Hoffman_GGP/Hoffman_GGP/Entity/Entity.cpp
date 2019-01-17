#include "../stdafx.h"

#include "Entity.h"
#include "../Resources/Mesh.h"
#include "../Resources/Materials/Material.h"
#include "../Resources/SimpleShader.h"
#include "../ECS/Component.h"

struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;

using namespace DirectX;

size_t Entity::EntityCount = 0;

Entity::Entity( Mesh* aMesh, Material* aMat, std::string aName )
    : EntityMesh( aMesh ), EntityMaterial( aMat ), Name( aName )
{    
    IsActive = true;

    entID = EntityCount++;

    componentManager = ECS::ComponentManager::GetInstance();

    // Give entity component a transform 
    EntityTransform = this->AddComponent<Transform>();
}

Entity::Entity()
{
    IsActive = true;

    entID = EntityCount++;
    componentManager = ECS::ComponentManager::GetInstance();

    // Give entity component a transform 
    EntityTransform = this->AddComponent<Transform>();
}

// virtual destructor
Entity::~Entity()
{
    EntityTransform = nullptr;
    EntityMesh = nullptr;
    EntityMaterial = nullptr;
    componentManager = nullptr;
}

void Entity::MoveRelative( const float aX, const float aY, const float aZ )
{
    // rotate desired movement by our rotation
    XMVECTOR dir = XMVector3Rotate(
        XMVectorSet( aX, aY, aZ, 0 ),
        XMLoadFloat4( &Rotation ) );

    // Add to position and store
    XMStoreFloat3(
        &Position,
        XMLoadFloat3( &Position ) + dir );
}

void Entity::PrepareMaterial( const VEC4x4 & aView, const VEC4x4 & aProjection )
{
    assert( EntityMaterial != nullptr );

    // Render all meshes that are a part of this entity
    // in the future I want to experiment with different meshes/material 
    // settings
    EntityMaterial->SetShaderValues( EntityTransform->GetWorldMatrix() , aView, aProjection);
}

void Entity::SaveObject( nlohmann::json & aOutFile )
{
    aOutFile[ "Entities" ][ Name ][ "POS" ][ "X" ] = Position.x;
    aOutFile[ "Entities" ][ Name ][ "POS" ][ "Y" ] = Position.y;
    aOutFile[ "Entities" ][ Name ][ "POS" ][ "Z" ] = Position.z;

    aOutFile[ "Entities" ][ Name ][ "ROT" ][ "X" ] = Rotation.x;
    aOutFile[ "Entities" ][ Name ][ "ROT" ][ "Y" ] = Rotation.y;
    aOutFile[ "Entities" ][ Name ][ "ROT" ][ "Z" ] = Rotation.z;
    aOutFile[ "Entities" ][ Name ][ "ROT" ][ "W" ] = Rotation.w;

    // Save each component
    auto compMap = this->GetAllComponents();

    if ( compMap != nullptr )
    {
        for ( auto compItr = compMap->begin(); compItr != compMap->end(); ++compItr )
        {
            ECS::IComponent* theComp = ( compItr->second );
            if ( theComp != nullptr )
            {
                theComp->SaveObject( aOutFile );
            }
        }
    }
}

////////////////////////////////////////////////////
// Accessors
////////////////////////////////////////////////////

Mesh * Entity::GetEntityMesh() const
{
    return EntityMesh;
}

const Material* Entity::GetMaterial() const
{
    return EntityMaterial;
}

void Entity::SetIsActive( const bool aStatus )
{
    IsActive = aStatus;
}

const bool Entity::GetIsActive() const
{
    return IsActive;
}

const std::string & Entity::GetName() const
{
    return Name;
}

void Entity::SetName( std::string newName )
{
    Name = newName;
}