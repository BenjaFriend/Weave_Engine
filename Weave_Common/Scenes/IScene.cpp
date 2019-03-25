#include "stdafx.h"
#include "IScene.h"

IScene::IScene()
{

}

IScene::~IScene()
{
}

void IScene::Write( OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState ) const
{
    ( void ) inOutputStream;
    ( void ) inDirtyState;
}

void IScene::Read( InputMemoryBitStream & inInputStream )
{
    ( void ) inInputStream;
}