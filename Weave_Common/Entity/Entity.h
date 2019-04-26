#pragma once

#include "stdafx.h"
#include "MemoryBitStream.h"
#include "Transform.h"
#include "ECS/ComponentManager.h"
#include "Utils/IPoolable.h"
#include "Utils/SaveFileDefs.h"
#include "Resources/ISaveable.h"

#include <memory>

/////////////////////////////////////////////////
// Forward Declarations
class Component;

/// <summary>
/// Base class for an entity in the engine. Any game object. Inherit from this 
/// class and create a 
/// </summary>
class Entity : public IPoolable< Entity >, public ISaveable
{
public:

	enum EIEntityReplicationState
	{
		EIRS_POS = 1 << 0,
		EIRS_ROT = 1 << 1,
		EIRS_AllState = EIRS_POS | EIRS_ROT
	};

	float packetTripTime = 0.0f;

	Entity();

	Entity(const std::string & aName);

	virtual ~Entity();

	/// <summary>
	/// Save this entity's data and components into a json array of 
	/// entities
	/// </summary>
	/// <param name="aJsonEntityArray">nlohmann::json::array of entities</param>
	void SaveObject(nlohmann::json& aJsonEntityArray) override;

	/// <summary>
	/// Create this entity based off of a scene file data set
	/// </summary>
	/// <param name="aFile">The set of data about this entity</param>
	Entity* ConstructFromFile(nlohmann::json const & aFile);


	// Components ------------------------------

	/// <summary>
	/// Get a pointer to this component on this entity
	/// </summary>
	/// <returns>
	/// Pointer to the component if it exists, nullptr 
	/// if this entity does not have a component of this type
	/// </returns>
	template<typename T>
	T* GetComponent()
	{
		return this->componentManager->GetComponent<T>(this->Pool_ID);
	}

	/// <summary>
	/// Add a component of type T to this entity
	/// </summary>
	template<class T, class ...P>
	T* AddComponent(P&&... param)
	{
		return
			this->componentManager->AddComponent<T>(
				this,
				this->Pool_ID,
				std::forward<P>(param)...
				);
	}

	/// <summary>
	/// Remove the component of this type from this entity
	/// </summary>
	template<typename T>
	void RemoveComponent()
	{
		this->componentManager->RemoveComponent<T>(this->Pool_ID);
	}

	FORCE_INLINE const ECS::ComponentMap * GetAllComponents() const
	{
		return componentManager->GetAllComponents(this->Pool_ID);
	}

	FORCE_INLINE void RemoveAllComponents() { componentManager->RemoveAllEntityComponents(Pool_ID); }

	virtual void Reset() override;

	/// <summary>
	/// Update this entity and it's components this frame
	/// </summary>
	/// <param name="dt">Delta time</param>
	virtual void Update(float dt);

	/// <summary>
	/// Write this component to a replicated bit stream
	/// </summary>
	/// <param name="inOutputStream"></param>
	/// <param name="inDirtyState"></param>
	virtual void Write(OutputMemoryBitStream & inOutputStream) const;

	/// <summary>
	/// Read this component from a bit stream
	/// </summary>
	/// <param name="inInputStream"></param>
	virtual void Read(InputMemoryBitStream & inInputStream);

protected:

	/// Ideally this would be moved to a networking transform/component
	/// Since client side entities don't necessarily need to have
	/// interpolation applied to changes with the transform
	struct Interpolation
	{
	public:

		/// Starting and ending position to lerp between
		glm::vec3 finalPos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 startPos = glm::vec3(0.0f, 0.0f, 0.0f);

		/// Starting and ending rotations to lerp between
		glm::vec3 finalRot = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 startRot = glm::vec3(0.0f, 0.0f, 0.0f);

		/// Lerp times
		float packetTime = 0.0f;
		float lerpTime = 0.1f;

		Interpolation()
		{
		}

		/// Interpolate the transform between starting and finishing values
		void Update(float deltaTime, Transform& transform)
		{
			// Return since the interpolation finished
			if (lerpTime >= packetTime)
			{
				return;
			}

			// Calculate the lerp value based on the time
			// between the packets to arrive
			lerpTime += deltaTime;
			float lerp = glm::clamp(lerpTime / glm::clamp(packetTime, 0.1f, packetTime), 0.0f, 1.1f);

			if (lerp >= 1.0f)
			{
				startPos = finalPos;
				startRot = finalRot;

				lerp = 1.0f;
			}

			// Calculate the lerp values and update the transform
			glm::vec3 posLerp = glm::lerp(startPos, finalPos, lerp);
			glm::vec3 rotLerp = glm::lerp(startRot, finalRot, lerp);
			transform.SetRotation(rotLerp);
			transform.SetPosition(posLerp);
		}

		/// Set the position of the transform and don't interpolate
		void SetTransform(Transform& transform)
		{
			startPos = finalPos;
			startRot = finalRot;

			transform.SetRotation(finalPos);
			transform.SetPosition(finalRot);
		}
	};

	Interpolation interpolate;

	virtual void WriteUpdateAction(OutputMemoryBitStream& inOutputStream, UINT32 inDirtyState) const;

	virtual void ReadUpdateAction(InputMemoryBitStream& inInputStream);

	static size_t EntityCount;

	INT32 NetworkID = -1;

	/** handles the adding/removing of components for this entity */
	ECS::ComponentManager * componentManager = nullptr;

	/** The transform of the entity */
	Transform* EntityTransform = nullptr;

	/** If this entity is dirty of not */
	UINT32 DirtyState = EIEntityReplicationState::EIRS_AllState;

	/** The replication action for this entity to take */
	EReplicationAction ReplicationAction = EReplicationAction::ERA_Create;

	// #TODO Have a proper type checking replication manager and entity registry system, this is not scalable
	/** The type of class that this entity is for replication */
	EReplicatedClassType ReplicatedClassType = EReplicatedClassType::EObstacle_Class;

	/** Flag for if this entity is active or not */
	UINT32 IsActive : 1;

	/** If true, then this entity will get destroyed when  */
	UINT32 IsDestroyableOnLoad : 1;

	/** If true, then this entity has been marked to be reset by one of it's components */
	UINT32 IsPendingReset : 1;

	/** The name of this object */
	std::string Name = "Default Entity";

	/************************************************************************/
	/* Accessors                                                            */
	/************************************************************************/

public:
	/** Get the current transform of this object */
	FORCE_INLINE Transform* GetTransform() const { return EntityTransform; }

	/// <summary>
	/// If an entity is destroyable on load, then it will be deleted during a 
	/// scene change. If not, then it will remain persistent throughout scenes
	/// </summary>
	/// <returns>True if destroyable</returns>
	FORCE_INLINE const bool GetIsDestroyableOnLoad() const { return IsDestroyableOnLoad; }

	FORCE_INLINE void SetIsDestroyableOnLoad(const bool aVal) { IsDestroyableOnLoad = aVal; }

	/// <summary>
	/// Sets if this entity is active or not
	/// </summary>
	/// <param name="aStatus">True if active, false if in-active</param>
	FORCE_INLINE void SetIsActive(const bool aStatus) { IsActive = aStatus; }

	/// <summary>
	/// Get if this entity is active or not
	/// </summary>
	/// <returns>True if active, false if in-active</returns>
	FORCE_INLINE const bool GetIsActive() const { return IsActive; }

	FORCE_INLINE const bool GetIsPendingReset() const { return IsPendingReset; }
	FORCE_INLINE void SetIsPendingReset(const bool val) { IsPendingReset = val; }

	/// <summary>
	/// Get this entity's name
	/// </summary>
	/// <returns>Reference to the name of this entity</returns>
	FORCE_INLINE const std::string & GetName() const { return Name; }

	/// <summary>
	/// Set the name of this entity
	/// </summary>
	/// <param name="newName">The new name of this entity</param>
	FORCE_INLINE void SetName(std::string newName) { Name = newName; }

	FORCE_INLINE const size_t GetID() const { return this->Pool_ID; }

	// Networked things things
	FORCE_INLINE const INT32 GetNetworkID() const { return NetworkID; }
	FORCE_INLINE void SetNetworkID(const INT32 aID) { NetworkID = aID; }

	FORCE_INLINE void SetDirtyState(const UINT32 aVal) { DirtyState = aVal; ReplicationAction = ERA_Update; }
	FORCE_INLINE UINT32 GetDirtyState() { return DirtyState; }
	FORCE_INLINE bool HasDirtyState() { return (DirtyState != 0); }

	FORCE_INLINE const EReplicationAction GetReplicationAction() { return ReplicationAction; }
	FORCE_INLINE void SetReplicationAction(const EReplicationAction aAct) { ReplicationAction = aAct; }

	FORCE_INLINE const EReplicatedClassType GetReplicationClassType() { return ReplicatedClassType; }
	FORCE_INLINE void SetReplicationClassType(EReplicatedClassType aRepClass) { ReplicatedClassType = aRepClass; }

};

// Use smart pointers for the client proxy to have safer exits
typedef std::shared_ptr< Entity >	IEntityPtr;