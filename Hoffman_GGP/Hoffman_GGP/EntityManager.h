#pragma once

#include <vector>

#include "Entity.h"
#include "DebugSettings.h"
#include "Definitions.h"

/////////////////////////////////////////////////
// Forward Declarations
class Mesh;
class Material;

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
    /// Adds an entity to the game with the given 
    /// </summary>
    /// <param name="aMesh">Mesh for this entity</param>
    /// <param name="aMat">Material of this entity</param>
    /// <returns>The ID of that entity</returns>
    const UINT AddEntity( Mesh* aMesh, Material* aMat );

    /// <summary>
    /// Adds an entity to the game with the given 
    /// </summary>
    /// <param name="aMesh">Mesh for this entity</param>
    /// <param name="aMat">Material of this entity</param>
    /// <param name="aPos">The starting position of the entity</param>
    /// <returns></returns>
    const UINT AddEntity( Mesh* aMesh, Material* aMat, const DirectX::XMFLOAT3 & aPos );


    /// <summary>
    /// Deletes entity with the given ID
    /// </summary>
    /// <param name="aEntityID">ID of the entity to delete</param>
    void DeleteEntity( const UINT aEntityID );

    /// <summary>
    /// Gets a pointer to an entity with this ID number
    /// </summary>
    /// <param name="aEntityID">Entity ID to find</param>
    /// <returns>pointer to the entity with this ID</returns>
    Entity* GetEntity( const UINT aEntityID ) const;

    //////////////////////////////////////////////////////////////////
    // Accessors 

    /// <summary>
    /// Get the current count of entities
    /// </summary>
    /// <returns>number of entities in the scene</returns>
    inline const size_t GetEntityCount() const;

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

