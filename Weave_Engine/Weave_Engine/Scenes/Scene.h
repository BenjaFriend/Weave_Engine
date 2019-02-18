#pragma once

#include "../stdafx.h"


#include <vector>

#include "../Entity/Entity.h"
#include "../Resources/Materials/Material.h"
#include "../Resources/Mesh.h"

using Entity_ID = size_t;

namespace SceneManagement
{
    /// <summary>
    /// A scene data structure for controlling what things are currently 
    /// enabled at runtime
    /// </summary>
    class Scene
    {
    public:

        Scene();

        ~Scene();

        /// <summary>
        /// Adds an entity to the game with no mesh. Adds this entity to the 
        /// vector of entities
        /// </summary>
        /// <param name="aName">Name of this entity</param>
        /// <returns>Pointer to this entity</returns>
        FORCE_INLINE Entity* AddEntity( std::string aName = "Default Entity" );

        /// <summary>
        /// Adds an entity and sets their position after it is created
        /// </summary>
        /// <param name="aName">Name of this entity</param>
        /// <param name="aPos">The spawn position</param>
        /// <returns>Pointer to the entity</returns>
        FORCE_INLINE Entity* AddEntity( std::string aName, glm::vec3 aPos );

        /// <summary>
        /// Load in an entity from some file information
        /// </summary>
        /// <param name="aFile"></param>
        /// <returns></returns>
        FORCE_INLINE Entity* AddEntityFromfile( nlohmann::json const & aFile );

        /// <summary>
        /// Deletes entity with the given ID
        /// </summary>
        /// <param name="aEntityID">ID of the entity to delete</param>
        FORCE_INLINE void DeleteEntity( Entity * aEntity );

        FORCE_INLINE const std::vector<Entity*> & GetEntityArray() { return EntityArray; }

        /// <summary>
        /// Get the current count of entities
        /// </summary>
        /// <returns>number of entities in the scene</returns>
        FORCE_INLINE const size_t GetEntityCount() const { return EntityArray.size(); }

        /// <summary>
        /// Get this scene's name
        /// </summary>
        /// <returns>Reference to the current scene</returns>
        FORCE_INLINE const std::string & GetSceneName() const { return SceneName; }

    private:

        /// <summary>
        /// Removes all currently loaded entities from the array 
        /// and deletes them
        /// </summary>
        void UnloadAllEntities();

        /** This scene's name */
        std::string SceneName = "DEFAULT_SCENE";

        /** Keep track of all entities in the scene */
        std::vector<Entity*> EntityArray;
    };
}