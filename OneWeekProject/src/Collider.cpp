#include "Collider.h"
#include "Entity.h"

Entity* ColliderBase::GetEntity() const
{
	return m_Entity;
}

void ColliderBase::SetOffset(Vec2f offset)
{
	m_Offset = offset;
}

Vec2f ColliderBase::GetOffset() const
{
	return m_Offset;
}

Vec2f ColliderBase::GetPosition() const
{
	return m_Entity->GetPosition() + m_Offset;
}

void ColliderBase::SetSize(Vec2f size)
{
	m_Size = size;
}

Vec2f ColliderBase::GetSize() const
{
	return m_Size;
}

Vec2f ColliderBase::GetHalfSize() const
{
	return { m_Size.x * 0.5f, m_Size.y * 0.5f };
}

void ColliderBase::EvaluateCollision(Entity* other)
{
	if (other && other->GetCollider())
	{
		EvaluateCollision( *(other->GetCollider()) );
	}
}
void ColliderBase::EvaluateCollision(ColliderBase& other)
{
	if (IsColliding(other))
	{
		Entity* otherEntity = other.GetEntity();
		m_Entity->OnCollision(*otherEntity);
		otherEntity->OnCollision(*m_Entity);
	}
}

bool ColliderBase::IsColliding(const ColliderBase& other)
{
	switch (other.GetType())
	{
		case EColliderTypes::CIRCLE:
			return IsCollidingCircle(other);
		break;
		case EColliderTypes::AABB:
			return IsCollidingAABB(other);
		break;
	}

	return false;
}









float ColliderCircle::GetRadius() const
{
	return GetSize().x * 0.5f;
}

bool ColliderCircle::IsCollidingCircle(const ColliderBase& other) const
{
	return Math::EvaluateCollisionCircles(GetPosition(), GetHalfSize().x, other.GetPosition(), other.GetHalfSize().x);
}

bool ColliderCircle::IsCollidingAABB(const ColliderBase& other) const
{
	return Math::EvaluateCollisionCircleAABB(GetPosition(), GetHalfSize().x, other.GetPosition(), other.GetHalfSize());

}







bool ColliderAABB::IsCollidingCircle(const ColliderBase& other) const
{
	return Math::EvaluateCollisionCircleAABB(
		other.GetPosition(), other.GetHalfSize().x, 
		GetPosition(), GetHalfSize()
	);
}

bool ColliderAABB::IsCollidingAABB(const ColliderBase& other) const
{
	return Math::EvaluateCollisionAABBs(
		GetPosition(), GetHalfSize(),
		other.GetPosition(), other.GetHalfSize()
	);
}


