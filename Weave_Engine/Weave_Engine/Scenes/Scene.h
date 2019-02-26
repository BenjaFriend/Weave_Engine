#pragma once

#include <vector>
#include <string>

#include "../Resources/Materials/Material.h"
#include "../Resources/Mesh.h"
#include "../Resources/SimpleShader.h"
#include "../Lighting/LightShaderDefs.h"
#include "../Lighting/PointLight.h"
#include "../Lighting/DirLight.h"

class Entity;

namespace SceneManagement
{

    using Entity_ID = size_t;

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
        /// Set data about this 
        /// </summary>
        /// <param name="aVertShader">The vert shader to use</param>
        /// <param name="aPixShader"></param>
        void SetShaderInfo( SimpleVertexShader* aVertShader, SimplePixelShader* aPixShader );

        /// <summary>
        /// Add a directional light to this system
        /// </summary>
        /// <param name="aDirLight">The dir light to be added</param>
        void AddDirLight( DirLight * aDirLight );

        /// <summary>
        /// Add a point light to this render system
        /// </summary>
        /// <param name="aPointLight">The point light to be added</param>
        void AddPointLight( PointLight * aPointLight );

        /// <summary>
        /// Remove all entities and lights in the current scene
        /// </summary>
        void ResetScene();

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

        FORCE_INLINE void SetSceneName( std::string aName ) { SceneName = aName; }

        FORCE_INLINE Entity* GetEntity( Entity_ID aID ) { if ( aID < 0 || aID > EntityArray.size() ) return nullptr;  return EntityArray [ aID ]; }

        FORCE_INLINE const std::vector<DirLight*> & GetDirLights() const { return DirLights; }

        FORCE_INLINE const std::vector<PointLight*> & GetPointLights() const { return PointLights; }

    private:

        /// <summary>
        /// Removes all currently loaded entities from the array 
        /// and deletes them
        /// </summary>
        void UnloadAllEntities();

        /// <summary>
        /// Remove all lights from this light system
        /// </summary>
        void UnloadAllLights();

        /// <summary>
        /// Send all lighting information to the given shaders
        /// </summary>
        /// <param name="aPixShader">Pixel shader to send lighting info to</param>
        void SetLightData( SimplePixelShader* aPixShader );


        /** This scene's name */
        std::string SceneName = "DEFAULT_SCENE";

        /** Keep track of all entities in the scene */
        std::vector<Entity*> EntityArray;

        std::vector<DirLight*> DirLights;

        std::vector<PointLight*> PointLights;
    };
}