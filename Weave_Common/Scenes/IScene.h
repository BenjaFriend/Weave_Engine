#pragma once

#include "Entity/IEntity.h"
#include "MemoryBitStream.h"
#include "Utils/ObjectPool.hpp"

class IScene
{
public:

    IScene();

    virtual ~IScene();

    virtual void Write( OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState ) const;
    virtual void Read( InputMemoryBitStream& inInputStream );

protected:

    /** This scene's name */
    std::string SceneName = "DEFAULT_SCENE";

};
