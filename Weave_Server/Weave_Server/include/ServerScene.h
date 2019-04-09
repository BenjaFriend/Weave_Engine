#pragma once

#include "Scenes/IScene.h"
#include "Entity/IEntity.h"

#include <vector>

class ServerScene : public IScene
{
public:
    ServerScene();

    virtual ~ServerScene();

    virtual void Write( OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState ) const override;

    /// <summary>
    /// Udpate the server scenes
    /// </summary>
    /// <param name="deltaTime"></param>
    /// <param name="totalTime"></param>
    virtual void Update( float deltaTime, float totalTime ) override;

    IEntity* AddEntity( const std::string & aName, UINT32 aID, const EReplicatedClassType aClassType );

private:

    std::vector< IEntityPtr > EntityArray;

    ObjectPool< IEntity >* EntityPool = nullptr;

};