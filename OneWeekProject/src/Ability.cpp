#include "Ability.h"
#include "Player.h"

Ability::Ability(const AbilityConstructionData& constructionData)
{
	m_Player = constructionData.AssignedToPlayer;
	m_Duration = constructionData.Duration;
	m_RemainingDuration = m_Duration;
}

Ability::~Ability()
{
}

void Ability::OnAdded(Player* player)
{
	m_Player = player;
}

void Ability::OnRemoved(Player* player)
{
	m_Player = nullptr;
}

void Ability::RefreshLifeTime()
{
	m_RemainingDuration = m_Duration;
}

void Ability::Remove()
{
	if (m_Player)
	{
		m_Player->RemoveAbility(this);
	}
}

void Ability::Update(float deltaTime)
{
	if (m_RemainingDuration < 0)
	{
		return;
	}

	m_RemainingDuration -= deltaTime;
	if (m_RemainingDuration < 0)
	{
		Remove();
	}
}
