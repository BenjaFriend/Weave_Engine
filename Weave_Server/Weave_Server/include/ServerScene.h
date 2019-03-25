#pragma once

#include "Scenes/IScene.h"

class ServerScene : public IScene
{
public:
    ServerScene();

    virtual ~ServerScene();

    virtual void Write( OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState ) const override;
    virtual void Read( InputMemoryBitStream& inInputStream ) override;

};