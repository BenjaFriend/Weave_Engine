#include "stdafx.h"
#include "ServerScene.h"

ServerScene::ServerScene()
{
    LOG_TRACE( "Create a server scene!" );
}

ServerScene::~ServerScene()
{
    EntityArray.clear();

    LOG_TRACE( "DELETE a server scene!" );
}

void ServerScene::Write( OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState ) const
{
    inOutputStream.Write( static_cast< UINT32 > ( EntityArray.size() ) );

    for ( size_t i = 0; i < EntityArray.size(); ++i )
    {
        EntityArray [ i ]->Write( inOutputStream, 0 );
    }
}
IEntityPtr ServerScene::AddEntity( const std::string & aName )
{
    IEntityPtr newEnt = std::make_shared<IEntity>();
    newEnt->SetName( aName );

    EntityArray.push_back( newEnt );

    return newEnt;
}
