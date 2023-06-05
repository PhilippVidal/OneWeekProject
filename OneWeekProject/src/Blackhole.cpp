#include "Blackhole.h"
#include "Player.h"
#include "Game.h"


Blackhole::Blackhole(const BlackholeConstructionData& constructionData)
	: Entity(constructionData)
{
	SetOuterSprite(GetGame()->GetSprite(constructionData.SpritePathOuter));

	m_Radius = GetOuterSprite() ?
		GetGame()->GetSpriteWorldSize(GetOuterSprite(), GetScale()).x :
		GetSize().x;

	m_Force = constructionData.Force;
}

void Blackhole::Update(float deltaTime)
{
	Player* player = static_cast<Player*>(GetGame()->GetPlayer());
	if (!player)
	{
		return;
	}

	Vec2f playerToHole = GetPosition() - player->GetPosition();
	float distance = playerToHole.Magnitude();
	if (distance < m_Radius)
	{
		float influenceFactor = 1.0f - (distance / m_Radius);
		Vec2f velocity = (playerToHole / distance) * influenceFactor * deltaTime * m_Force;
		player->AddVelocity(velocity);
	}
}

void Blackhole::Draw() const
{
	Entity::Draw();

	if (GetOuterSprite())
	{
		GetGame()->DrawWorld(GetOuterSprite(), GetPosition(), GetGame()->GetSpriteWorldSize(GetOuterSprite(), GetScale()));
	}
}

void Blackhole::OnCollision(Entity& other)
{
	Entity::OnCollision(other);

	if (other.GetType() == EObjectTypes::PLAYER)
	{
		Player* player = static_cast<Player*>(&other);		
		if (player->CanDie())
		{
			player->Die();
		}
	}
}
