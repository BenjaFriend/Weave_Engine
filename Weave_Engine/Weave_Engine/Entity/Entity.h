#pragma once

#include "stdafx.h"
#include "Entity/IEntity.h"
#include "Resources/ISaveable.h"
#include "Utils/SaveFileDefs.h"

/////////////////////////////////////////////////
// Forward Declarations
class Component;

/// <summary>
///  Represents a game entity and provides access to their components
/// </summary>
/// <author>Ben Hoffman</author>
class Entity : public ISaveable, public IEntity
{

public:

    /// <summary>
    /// Default constructor for this entity
    /// </summary>
    Entity();

    /// <summary>
    /// Creates an empty game object with the given name
    /// </summary>
    /// <param name="aName">Name of this entity</param>
    Entity( std::string aName );

    virtual ~Entity();

    /// <summary>
    /// Create this entity based off of a scene file data set
    /// </summary>
    /// <param name="aFile">The set of data about this entity</param>
    Entity* ConstructFromFile( nlohmann::json const & aFile );

    /// <summary>
    /// Save this entity's data and components into a json array of 
    /// entities
    /// </summary>
    /// <param name="aJsonEntityArray">nlohmann::json::array of entities</param>
    void SaveObject( nlohmann::json& aJsonEntityArray );

};