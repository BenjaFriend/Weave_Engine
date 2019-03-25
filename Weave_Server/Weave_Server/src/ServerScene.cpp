#include "stdafx.h"
#include "ServerScene.h"

ServerScene::ServerScene()
{

}

ServerScene::~ServerScene()
{
}

void ServerScene::Write( OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState ) const
{
    // Foreach entity in the scene
        // If it is dirty
            // Write out it's info!
}

void ServerScene::Read( InputMemoryBitStream & inInputStream )
{

}