#include "../stdafx.h"

#include "IComponent.h"

using namespace ECS;

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
    return factory->second->constructFromFile( comp_data );
}