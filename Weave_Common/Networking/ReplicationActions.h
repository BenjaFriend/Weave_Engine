#pragma once

enum EReplicationAction
{
    ERA_Create,
    ERA_Update,
    ERA_Destroy
};

// This is a temporary solution instead of doing additional
// component type check replication stuff. It's not scalable
// for the engine and should be refactored out in the next
// pass
enum EReplicatedClassType
{
	ETank_Class,
	EBullet_Class,
	EObstacle_Class
};