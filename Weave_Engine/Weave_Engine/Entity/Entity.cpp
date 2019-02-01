#include "../stdafx.h"

#include "Entity.h"
#include "../Resources/Mesh.h"
#include "../Resources/Materials/Material.h"
#include "../Resources/SimpleShader.h"
#include "../ECS/Component.h"

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

void Entity::PrepareMaterial( const glm::highp_mat4 & aView, const glm::highp_mat4 & aProjection )
{
    assert( EntityMaterial != nullptr );

    // Render all meshes that are a part of this entity
    // in the future I want to experiment with different meshes/material 
    // settings
    EntityMaterial->SetShaderValues( EntityTransform->GetWorldMatrix() , aView, aProjection);
}

void Entity::SaveObject( nlohmann::json & aOutFile )
{
    LOG_TRACE( "Save Object: {}", this->Name );
    // Save each component
    const auto & compMap = this->GetAllComponents();

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