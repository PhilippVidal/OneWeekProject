#pragma once
#include "Vec2.h"
#include "Math.h"

enum class EColliderTypes : unsigned char
{
	NONE,
	CIRCLE,
	AABB
};

// It could be useful to keep track of which objects we are currently colliding with
// i.e., have separate OnCollisionStart, OnCollisionEnd, OnCollisionContinue functions
// but for this basic gameplay loop having a simple OnCollision function is probably enough
class ColliderBase
{
	class Entity* m_Entity = nullptr;
	Vec2f m_Offset = { 0.f, 0.f };
	Vec2f m_Size = { 1.f, 1.f };


public:
	ColliderBase(class Entity* entity, Vec2f size, Vec2f offset = {0.f, 0.f})
		: m_Entity(entity), m_Size(size), m_Offset(offset){}

	Entity* GetEntity() const;
	void SetOffset(Vec2f offset);
	Vec2f GetOffset() const;
	Vec2f GetPosition() const;

	void SetSize(Vec2f size);
	Vec2f GetSize() const;
	Vec2f GetHalfSize() const;
	
	virtual EColliderTypes GetType() const = 0;

	virtual void EvaluateCollision(Entity* other);
	virtual void EvaluateCollision(ColliderBase& other);

	virtual bool IsColliding(const ColliderBase& other);

	virtual bool IsCollidingCircle(const ColliderBase& other) const = 0;
	virtual bool IsCollidingAABB(const ColliderBase& other) const = 0;

	
};


class ColliderCircle : public ColliderBase
{
public:

	ColliderCircle(class Entity* entity, float radius, Vec2f offset = { 0.f, 0.f })
		: ColliderBase(entity, { radius, radius }, offset) {}

	float GetRadius() const;

	virtual EColliderTypes GetType() const override
	{
		return EColliderTypes::CIRCLE;
	}

	virtual bool IsCollidingCircle(const ColliderBase& other) const override;
	virtual bool IsCollidingAABB(const ColliderBase& other) const override;
};


class ColliderAABB : public ColliderBase
{
public:
	ColliderAABB(class Entity* entity, Vec2f size, Vec2f offset = { 0.f, 0.f })
		: ColliderBase(entity, size, offset) {}

	virtual EColliderTypes GetType() const override
	{
		return EColliderTypes::AABB;
	}

	virtual bool IsCollidingCircle(const ColliderBase& other) const override;
	virtual bool IsCollidingAABB(const ColliderBase& other) const override;
};

