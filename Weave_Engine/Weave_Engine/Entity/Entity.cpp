#include "../stdafx.h"

#include "Entity.h"
#include "../Resources/Mesh.h"
#include "../Resources/Materials/Material.h"
#include "../Resources/SimpleShader.h"
#include "../ECS/Component.h"

size_t Entity::EntityCount = 0;

Entity::Entity( std::string aName )
    : Entity()
{
    Name = aName;
}

Entity::Entity( std::string aName, glm::vec3 aPos )
    : Entity( aName )
{
    if ( EntityTransform == nullptr )
    {
        EntityTransform = this->AddComponent<Transform>();
    }
    EntityTransform->SetPosition( aPos );
}

Entity::Entity()
{
    IsActive = true;
    IsValid = false;
    IsDestroyableOnLoad = true;

    entID = EntityCount++;
    componentManager = ECS::ComponentManager::GetInstance();

    // Give entity component a transform 
    if ( EntityTransform == nullptr )
    {
        EntityTransform = this->AddComponent<Transform>();
    }
}

// virtual destructor
Entity::~Entity()
{
    // Remove my components
    RemoveAllComponents();
    EntityTransform = nullptr;
    componentManager = nullptr;
    --EntityCount;
}

Entity * Entity::ConstructFromFile( nlohmann::json const & aFile )
{
    RemoveAllComponents();

    Name = aFile [ NAME_SAVE_KEY ];
    IsActive = aFile [ IS_ACTIVE_SAVE_KEY ];
    IsDestroyableOnLoad = aFile [ IS_DESTROY_ON_LOAD ];
    nlohmann::json comp_data = aFile [ COMPONENT_ARRAY_SAVE_KEY ];

    nlohmann::json::iterator compItr = comp_data.begin();

    while ( compItr != comp_data.end() )
    {
        componentManager->AddComponent( this, *compItr );
        ++compItr;
    }
    EntityTransform = this->GetComponent<Transform>();

    LOG_TRACE( "Load Entity from file: {} \t Active: {}", Name, IsActive );

    return this;
}

void Entity::SaveObject( nlohmann::json & aJsonEntityArray )
{
    LOG_TRACE( "Save Object: {}", this->Name );

    // Save this entity's data
    nlohmann::json entity_data = nlohmann::json::object();

    entity_data [ NAME_SAVE_KEY ] = this->Name;
    entity_data [ IS_ACTIVE_SAVE_KEY ] = this->IsActive;
    entity_data [ IS_DESTROY_ON_LOAD ] = ( bool ) this->IsDestroyableOnLoad;
    entity_data [ COMPONENT_ARRAY_SAVE_KEY ] = nlohmann::json::array();

    // Save each component
    const auto & compMap = this->GetAllComponents();

    if ( compMap != nullptr )
    {
        for ( auto compItr = compMap->begin(); compItr != compMap->end(); ++compItr )
        {
            ECS::IComponent* theComp = ( compItr->second );

            if ( theComp != nullptr )
            {
                theComp->SaveObject( entity_data [ COMPONENT_ARRAY_SAVE_KEY ] );
            }
        }
    }
    // Append this entity to the given entity array
    if ( aJsonEntityArray.is_array() )
    {
        aJsonEntityArray.push_back( entity_data );
    }
}

////////////////////////////////////////////////////
// Accessors
////////////////////////////////////////////////////

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