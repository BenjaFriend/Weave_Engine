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

    IEntityPtr AddEntity( const std::string & aName );

private:

    std::vector< IEntityPtr > EntityArray;

};