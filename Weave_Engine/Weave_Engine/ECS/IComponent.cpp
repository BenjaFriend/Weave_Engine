#include "../stdafx.h"

#include "IComponent.h"
#include "../Entity/Entity.h"

using namespace ECS;

void IComponent::SaveObject( nlohmann::json & aEntComponentArray )
{
    nlohmann::json comp_data = nlohmann::json::object();
    comp_data [ COMP_SAVE_KEY ] = ComponentName();

    SaveComponentData( comp_data );

    if ( aEntComponentArray.is_array() )
    {
        aEntComponentArray.push_back( comp_data );
    }
    else
    {
        LOG_WARN( "Could not save component data to the array!" );
    }
}


IComponent::Factory::Factory( std::string const & type )
{
    std::pair <FactoryMap::iterator, bool> results
        = ComponentFactories().insert( std::make_pair( type, this ) );
    assert( results.second );
}

IComponent* IComponent::ReadFromFile( nlohmann::json & comp_data )
{
    std::string compType = comp_data [ COMP_SAVE_KEY ];
    FactoryMap::const_iterator factory = ComponentFactories().find( compType );
    if ( factory == ComponentFactories().end() )
    {
        LOG_ERROR( "Unknown component type! {}", compType );
        return nullptr;
    }
    return factory->second->ConstructFromFile( comp_data );
}

IComponent * ECS::IComponent::ReadFromEditor( const std::string & aCompName )
{
    FactoryMap::const_iterator factory = ComponentFactories().find( aCompName );
    if ( factory == ComponentFactories().end() )
    {
        LOG_ERROR( "Unknown component type! {}", aCompName );
        return nullptr;
    }
    return factory->second->ConstructFromEditor();
}
