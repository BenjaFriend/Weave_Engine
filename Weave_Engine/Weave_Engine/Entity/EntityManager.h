#pragma once

#include "../stdafx.h"

#include <vector>
#include "json/json.hpp"
#include "Entity.h"
#include "../Resources/MeshRenderer.h"
#include "../Scenes/SceneManager.h"
#include "../Utils/Dispatcher.hpp"

/////////////////////////////////////////////////
// Forward Declarations
class Mesh;
class Material;

using Entity_ID = size_t;

/// <summary>
/// A singleton class that will  be in charge of creating and 
/// destroying entities 
/// </summary>
/// <author>Ben Hoffman</author>
class EntityManager
{
public:

    /// <summary>
    /// Get the current instance of the entity manager.
    /// If one does not exist, than create a new one.
    /// </summary>
    /// <returns>Static instance to of the entity manager</returns>
    static EntityManager* GetInstance();

    /// <summary>
    /// Release the current instance of the entity manager
    /// </summary>
    static void ReleaseInstance();

    /// <summary>
    /// Adds an entity to the game with no mesh. Adds this entity to the 
    /// vector of entities
    /// </summary>
    /// <param name="aName">Name of this entity</param>
    /// <returns>Pointer to this entity</returns>
    Entity* AddEntity( std::string aName = "Default Entity" );

    /// <summary>
    /// Adds an entity and sets their position after it is created
    /// </summary>
    /// <param name="aName">Name of this entity</param>
    /// <param name="aPos">The spawn position</param>
    /// <returns>Pointer to the entity</returns>
    Entity* AddEntity( std::string aName, glm::vec3 aPos );

    /// <summary>
    /// Load in an entity from some file information
    /// </summary>
    /// <param name="aFile"></param>
    /// <returns></returns>
    Entity* AddEntityFromfile( nlohmann::json const & aFile );

    /// <summary>
    /// Deletes entity with the given ID
    /// </summary>
    /// <param name="aEntityID">ID of the entity to delete</param>
    void DeleteEntity( Entity * aEntity );

    /// <summary>
    /// Gets a pointer to an entity with this ID number
    /// </summary>
    /// <param name="aEntityID">Entity ID to find</param>
    /// <returns>pointer to the entity with this ID</returns>
    FORCE_INLINE Entity* GetEntity( const Entity_ID aEntityID ) const
    {
        assert( aEntityID >= 0 && aEntityID < EntityArray.size() );

        return EntityArray [ aEntityID ];
    }

    //////////////////////////////////////////////////////////////////
    // Accessors 

    /// <summary>
    /// Get the current count of entities
    /// </summary>
    /// <returns>number of entities in the scene</returns>
    const size_t GetEntityCount() const;

    // We don't want anything making copies of this class so delete these operators
    EntityManager( EntityManager const& ) = delete;
    void operator=( EntityManager const& ) = delete;

private:

    EntityManager();

    ~EntityManager();

    /// <summary>
    /// Removes all currently loaded entities from the array 
    /// and deletes them
    /// </summary>
    void UnloadAllEntities();

    static EntityManager* Instance;

    /** Keep track of all entities in the scene */
    std::vector<Entity*> EntityArray;

};

