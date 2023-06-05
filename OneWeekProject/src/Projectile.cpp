#include "Projectile.h"
#include "Game.h"

Projectile::Projectile(const ProjectileConstructionData& constructionData)
	: Entity(constructionData)
{
	SetLifeTime(constructionData.LifeTime);
	SetPosition(constructionData.Position);
	SetVelocity(constructionData.Velocity);

	if (constructionData.FireOnConstruction)
	{
		Fire();
	}
}

void Projectile::Fire()
{
	m_IsFired = true;
}

void Projectile::Reset()
{
	m_IsFired = false;
	ResetLifeTime();
}

void Projectile::Update(float deltaTime)
{
	if (!IsFired())
	{
		return;
	}

	SetRemainingLifeTime(GetRemainingLifeTime() - deltaTime);
	if (GetRemainingLifeTime() < 0)
	{
		Destroy();
		return;
	}

	Vec2f newPosition = GetPosition() + GetVelocity() * deltaTime;
	newPosition.x = (newPosition.x > 0) ?
		fmodf(newPosition.x, GetGame()->GetWorldSize().x)
		: newPosition.x + GetGame()->GetWorldSize().x;

	SetPosition(newPosition);
}

void Projectile::OnCollision(Entity& other)
{
	if (other.GetType() == EObjectTypes::ENEMY)
	{
		other.Destroy();
	}

	Destroy();
}

void Projectile::ResetLifeTime()
{
	m_RemainingLifeTime = GetLifeTime();
}
