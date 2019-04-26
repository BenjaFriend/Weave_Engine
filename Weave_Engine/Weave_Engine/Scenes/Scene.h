#pragma once

#include <vector>
#include <string>

#include "stdafx.h"

#include "Scenes/IScene.h"
#include "Entity/Entity.h"
#include "Utils/ObjectPool.hpp"

#include "../Resources/Materials/Material.h"
#include "../Resources/Mesh.h"
#include "../Resources/SimpleShader.h"
#include "../Lighting/LightShaderDefs.h"
#include "../Lighting/PointLight.h"
#include "../Lighting/DirLight.h"

namespace SceneManagement
{

    using Entity_ID = UINT64;

    /// <summary>
    /// A scene data structure for controlling what things are currently 
    /// enabled at runtime
    /// </summary>
    class Scene : public IScene
    {
    public:

        Scene();

        virtual ~Scene();

        virtual void Read( InputMemoryBitStream& inInputStream ) override;

		float packetTripTime = 0;

        /// <summary>
        /// Adds an entity to the game with no mesh. Adds this entity to the 
        /// vector of entities
        /// </summary>
        /// <param name="aName">Name of this entity</param>
        /// <returns>Pointer to this entity</returns>
        Entity* AddEntity( std::string aName = "Default Entity" );

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
        virtual void ResetScene() override;

        FORCE_INLINE const std::vector<DirLight*> & GetDirLights() const { return DirLights; }

        FORCE_INLINE const std::vector<PointLight*> & GetPointLights() const { return PointLights; }

    private:

        /// <summary>
        /// Remove all lights from this light system
        /// </summary>
        void UnloadAllLights();

        /// <summary>
        /// Send all lighting information to the given shaders
        /// </summary>
        /// <param name="aPixShader">Pixel shader to send lighting info to</param>
        void SetLightData( SimplePixelShader* aPixShader );

        std::vector< DirLight* > DirLights;

        std::vector< PointLight* > PointLights;
    };
}