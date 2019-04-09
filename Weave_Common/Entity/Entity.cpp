#include "stdafx.h"
#include "Entity.h"

size_t Entity::EntityCount = 0;

Entity::Entity()
{
    IsActive = true;
    IsValid = false;
    IsDestroyableOnLoad = true;
    IsPendingReset = false;

    Pool_ID = EntityCount++;

    componentManager = ECS::ComponentManager::GetInstance();

    // Give entity component a transform 
    if ( EntityTransform == nullptr )
    {
        EntityTransform = this->AddComponent<Transform>();
    }
}

Entity::Entity( const std::string & aName )
    : Entity()
{
    Name = aName;
}

Entity::~Entity()
{
    --EntityCount;
    // Remove my components
    RemoveAllComponents();
    EntityTransform = nullptr;
    componentManager = nullptr;
    IsActive = true;
    IsValid = false;
}

void Entity::SaveObject( nlohmann::json & aJsonEntityArray )
{
    LOG_TRACE( "Save Object: {}", this->Name );

    // Save this entity's data
    nlohmann::json entity_data = nlohmann::json::object();

    entity_data[ NAME_SAVE_KEY ] = this->Name;
    entity_data[ IS_ACTIVE_SAVE_KEY ] = ( bool ) this->IsActive;
    entity_data[ IS_DESTROY_ON_LOAD ] = ( bool ) this->IsDestroyableOnLoad;
    entity_data[ COMPONENT_ARRAY_SAVE_KEY ] = nlohmann::json::array();

    // Save each component
    const auto & compMap = this->GetAllComponents();

    if ( compMap != nullptr )
    {
        for ( auto compItr = compMap->begin(); compItr != compMap->end(); ++compItr )
        {
            ECS::IComponent* theComp = ( compItr->second );

            if ( theComp != nullptr )
            {
                theComp->SaveObject( entity_data[ COMPONENT_ARRAY_SAVE_KEY ] );
            }
        }
    }
    // Append this entity to the given entity array
    if ( aJsonEntityArray.is_array() )
    {
        aJsonEntityArray.push_back( entity_data );
    }
}

Entity * Entity::ConstructFromFile( nlohmann::json const & aFile )
{
    RemoveAllComponents();

    Name = aFile[ NAME_SAVE_KEY ];
    IsActive = aFile[ IS_ACTIVE_SAVE_KEY ];
    IsDestroyableOnLoad = aFile[ IS_DESTROY_ON_LOAD ];
    nlohmann::json comp_data = aFile[ COMPONENT_ARRAY_SAVE_KEY ];

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


void Entity::Reset()
{
    IPoolable::Reset();

    RemoveAllComponents();
    // Reset flags
    IsValid = false;
    IsDestroyableOnLoad = true;
    IsPendingReset = false;

    // Ensure that we have a good instance of the component manager
    componentManager = ECS::ComponentManager::GetInstance();

    // Reset the transform
    EntityTransform = nullptr;    
    EntityTransform = this->AddComponent<Transform>();
}

void Entity::Update( float dt )
{
    // Update all this entities components
    const auto & compMap = this->GetAllComponents();

    assert( compMap != nullptr );

    for ( auto && compItr = compMap->begin(); compItr != compMap->end(); ++compItr )
    {
        if( compItr->second != nullptr )
            compItr->second->Update( dt );
    }

    // Check for if this entity has been marked for reset by one of it's components
    if ( IsPendingReset )
    {
        Reset();
    }
}

void Entity::Write( OutputMemoryBitStream & inOutputStream ) const
{
    inOutputStream.Write( NetworkID );
    // Write our replication action and the dirty state
    inOutputStream.Write( ReplicationAction, 2 );
    inOutputStream.Write( ReplicatedClassType, 2 );
    inOutputStream.Write( DirtyState );

    switch ( ReplicationAction )
    {
    case EReplicationAction::ERA_Create:
    {
        // If we are creating this object then we need to know it's type
        WriteUpdateAction( inOutputStream, DirtyState );
    }
    break;
    case EReplicationAction::ERA_Update:
    {
        WriteUpdateAction( inOutputStream, DirtyState );
    }
    break;
    case EReplicationAction::ERA_Destroy:
    {
        // If we are destroying this object, than we don't need to write anything else
    }
    break;
    }
}

void Entity::WriteUpdateAction( OutputMemoryBitStream & inOutputStream, UINT32 inDirtyState ) const
{
    if ( inDirtyState & EIEntityReplicationState::EIRS_POS )
    {
        const glm::vec3 & pos = EntityTransform->GetPosition();
        inOutputStream.Write( pos.x );
        inOutputStream.Write( pos.y );
        inOutputStream.Write( pos.z );
    }

    if ( inDirtyState & EIEntityReplicationState::EIRS_ROT )
    {
        const glm::vec3 & rot = EntityTransform->GetRotation();
        inOutputStream.Write( rot.x );
        inOutputStream.Write( rot.y );
        inOutputStream.Write( rot.z );
    }
}

void Entity::ReadUpdateAction( InputMemoryBitStream & inInputStream )
{
    // Read in pos
    if ( DirtyState & EIEntityReplicationState::EIRS_POS )
    {
        glm::vec3 readPos( 0.f );
        inInputStream.Read( readPos.x );
        inInputStream.Read( readPos.y );
        inInputStream.Read( readPos.z );

        EntityTransform->SetPosition( readPos );
    }

    if ( DirtyState & EIEntityReplicationState::EIRS_ROT )
    {
        glm::vec3 readRot( 0.0f );
        inInputStream.Read( readRot.x );
        inInputStream.Read( readRot.y );
        inInputStream.Read( readRot.z );
        EntityTransform->SetRotation( readRot );
    }
}

void Entity::Read( InputMemoryBitStream & inInputStream )
{
    // Read in dirty state of this entity
    inInputStream.Read( DirtyState );

    ReadUpdateAction( inInputStream );

    // #TODO: Write out each of this entities components
}