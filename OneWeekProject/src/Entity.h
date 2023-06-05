#pragma once
#include "Object.h"
#include "Collider.h"
#include "ECollisionLayers.h"
#include "EGameLayers.h"

#include <memory>


struct EntityConstructionData : public ObjectConstructionData
{
	Vec2f Position = { 0.0f, 0.0f };
	Vec2f Scale = { 1.0f, 1.0f };

	Vec2f Velocity = { 0.0f, 0.0f };
	bool IsStatic = false;
	EGameLayers GameLayer = EGameLayers::DEFAULT;
	EColliderTypes ColliderType = EColliderTypes::AABB;
	ECollisionLayers CollisionLayer = ECollisionLayers::ENTITY;
};


//Entity placed in the game world
class Entity : public Object
{
	Vec2f m_Position = { 0.0f, 0.0f };
	Vec2f m_Scale = { 1.0f, 1.0f };
	Vec2f m_Velocity = { 0.0f, 0.0f };

	bool m_IsStatic = false;
	bool m_IsCollisionEnabled = true;
	ECollisionLayers m_CollisionLayer = ECollisionLayers::ENTITY;
	EColliderTypes m_ColliderType = EColliderTypes::AABB;
	std::unique_ptr<ColliderBase> m_Collider = nullptr;

	EGameLayers m_GameLayer = EGameLayers::DEFAULT;

public:


	Entity(const EntityConstructionData& constructionData);

	virtual ~Entity() {};

	virtual void Update(float deltaTime) override;

	virtual void Draw() const override;



	void GetWorldBounds(float& minX, float& maxX, float& minY, float& maxY);
	virtual Vec2f GetWorldBoundsX() const;
	virtual Vec2f GetWorldBoundsY() const;

	Vec2f GetSize() const;
	void AddVelocity(const Vec2f& velocity);

	virtual bool IsBeneathGameScreen() const;

	virtual void OnCollision(Entity& other);
	bool CanCollideWithOther(const Entity& other);

	virtual void Destroy() override;

protected:

	virtual void CreateCollider();


public:

	virtual const EObjectTypes GetType() const override
	{
		return EObjectTypes::ENTITY;
	}

	void SetGameLayer(EGameLayers gameLayer)
	{
		m_GameLayer = gameLayer;
	}

	EGameLayers GetGameLayer() const
	{
		return m_GameLayer;
	}

	unsigned int GetGameLayerIndex() const
	{
		return static_cast<unsigned int>(GetGameLayer());
	}


	void SetCollisionLayer(ECollisionLayers layer)
	{
		m_CollisionLayer = layer;
	}

	ECollisionLayers GetCollisionLayer() const
	{
		return m_CollisionLayer;
	}


	void SetColliderType(EColliderTypes colliderType);
	

	EColliderTypes GetColliderType() const
	{
		return m_ColliderType;
	}

	template<class T>
	void SetCollider(T* collider)
	{
		m_Collider = std::make_unique<T>(std::move(collider));
	}

	ColliderBase* GetCollider() const
	{
		return m_Collider.get();
	}

	void EnableCollision(bool value)
	{
		m_IsCollisionEnabled = value;
	}

	bool IsCollisionEnabled() const
	{
		return m_IsCollisionEnabled;
	}


	void SetVelocity(float x, float y)
	{
		m_Velocity = { x, y };
	}

	void SetVelocity(const Vec2f& velocity)
	{
		m_Velocity = velocity;
	}

	Vec2f GetVelocity() const
	{
		return m_Velocity;
	}

	Vec2f GetScale() const
	{
		return m_Scale;
	}

	void SetScale(Vec2f scale)
	{
		m_Scale = scale;
	}

	bool IsStatic() const
	{
		return m_IsStatic;
	}

	void SetStatic(bool value)
	{
		m_IsStatic = value;
	}

	void SetPosition(float x, float y)
	{
		m_Position = { x, y };
	}

	void SetPosition(const Vec2f& position)
	{
		m_Position = position;
	}

	Vec2f GetPosition() const
	{
		return m_Position;
	}	
};

