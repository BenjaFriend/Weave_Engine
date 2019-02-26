#include "../stdafx.h"

#include "Entity.h"
#include "../Resources/Mesh.h"
#include "../Resources/Materials/Material.h"
#include "../Resources/SimpleShader.h"
#include "../ECS/Component.h"

size_t Entity::EntityCount = 0;

Entity::Entity( std::string aName )
    : Name( aName )
{
    IsActive = true;

    entID = EntityCount++;

    componentManager = ECS::ComponentManager::GetInstance();

    // Give entity component a transform 
    EntityTransform = this->AddComponent<Transform>();
}

Entity::Entity( std::string aName, glm::vec3 aPos )
    : Entity( aName )
{
    EntityTransform->SetPosition( aPos );
}

Entity::Entity( nlohmann::json const & aFile )
{
    Name = aFile [ NAME_SAVE_KEY ];
    IsActive = aFile [ IS_ACTIVE_SAVE_KEY ];

    entID = EntityCount++;

    componentManager = ECS::ComponentManager::GetInstance();

    // #TODO: Load components
    nlohmann::json comp_data = aFile [ COMPONENT_ARRAY_SAVE_KEY ];

    nlohmann::json::iterator compItr = comp_data.begin();

    while ( compItr != comp_data.end() )
    {
        componentManager->AddComponent( entID, *compItr );
        ++compItr;
    }
    EntityTransform = this->GetComponent<Transform>();

    LOG_TRACE( "Load Entity from file: {} \t Active: {}", Name, IsActive );
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
    // Remove my components
    componentManager->RemoveAllEntityComponents( entID );
    EntityTransform = nullptr;
    componentManager = nullptr;
    --EntityCount;
}

void Entity::SaveObject( nlohmann::json & aJsonEntityArray )
{
    LOG_TRACE( "Save Object: {}", this->Name );

    // Save this entity's data
    nlohmann::json entity_data = nlohmann::json::object();

    entity_data [ NAME_SAVE_KEY ] = this->Name;
    entity_data [ IS_ACTIVE_SAVE_KEY ] = this->IsActive;
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