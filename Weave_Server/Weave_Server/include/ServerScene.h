#pragma once

#include "Scenes/IScene.h"
#include "Entity/Entity.h"

#include <vector>

class ServerScene : public IScene
{
public:
    ServerScene();

    virtual ~ServerScene();

    virtual void Write( OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState ) const override;

    Entity* AddEntity( const std::string & aName, UINT32 aID, const EReplicatedClassType aClassType );

private:

};