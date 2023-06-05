#include "Entity.h"
#include "Game.h"
#include "SpriteObject.h"

Entity::Entity(const EntityConstructionData& constructionData)
	: Object(constructionData)
{
	m_Collider = nullptr;
	SetScale(constructionData.Scale);
	SetPosition(constructionData.Position);
	SetVelocity(constructionData.Velocity);
	SetGameLayer(constructionData.GameLayer);
	SetCollisionLayer(constructionData.CollisionLayer);
	SetColliderType(constructionData.ColliderType);
	CreateCollider();
}

void Entity::Update(float deltaTime)
{
	Object::Update(deltaTime);

	if (IsBeneathGameScreen())
	{
		Destroy();
	}
}

void Entity::Draw() const
{
	if (SetDefaultSprite())
	{
		GetGame()->DrawWorld(SetDefaultSprite(), GetPosition(), GetSize());
	}
}

void Entity::GetWorldBounds(float& minX, float& maxX, float& minY, float& maxY)
{
	Vec2f boundsX = GetWorldBoundsX();
	Vec2f boundsY = GetWorldBoundsY();

	minX = boundsX.x;
	maxX = boundsX.y;

	minY = boundsY.x;
	maxY = boundsY.y;
}

Vec2f Entity::GetWorldBoundsX() const
{
	Vec2f position = GetPosition();
	Vec2f halfSize = GetSize() * 0.5f;
	return { position.x - halfSize.x, position.x + halfSize.x };
}


Vec2f Entity::GetWorldBoundsY() const
{
	Vec2f position = GetPosition();
	Vec2f halfSize = GetSize() * 0.5f;
	return Vec2f(position.y - halfSize.y, position.y + halfSize.y);
}

Vec2f Entity::GetSize() const
{
	return GetGame()->GetSpriteWorldSize(SetDefaultSprite(), GetScale());
}

bool Entity::IsBeneathGameScreen() const
{
	return GetWorldBoundsY().y < GetGame()->GetCameraPosition().y;
}

void Entity::AddVelocity(const Vec2f& velocity)
{
	SetVelocity(GetVelocity() + velocity);
}

void Entity::CreateCollider()
{
	switch (GetColliderType())
	{
		case EColliderTypes::CIRCLE:
		{
			Vec2f size = GetSize();
			m_Collider = std::make_unique<ColliderCircle>(this, fmaxf(size.x, size.y));
		}
		break;
		case EColliderTypes::AABB:
		{
			m_Collider = std::make_unique<ColliderAABB>(this, GetSize());
		}
		break;
	}
}

void Entity::OnCollision(Entity& other)
{

}

bool Entity::CanCollideWithOther(const Entity& other)
{
	return ( GetGame()->GetCollisionMask(GetCollisionLayer()) >> static_cast<CollisionMask>(other.GetCollisionLayer()) ) & 1;
}

void Entity::Destroy()
{
	Object::Destroy();
	EnableCollision(false);
	SetIsVisible(false);
}

void Entity::SetColliderType(EColliderTypes colliderType)
{
	if (m_ColliderType != colliderType)
	{
		m_ColliderType = colliderType;
		CreateCollider();
	}
}
