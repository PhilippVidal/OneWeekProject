#include "AbilityPickup.h"
#include "Player.h"
#include "Ability.h"

AbilityPickup::AbilityPickup(const EntityConstructionData& constructionData)
	: Entity(constructionData)
{

}

void AbilityPickup::GrantAbility(Player* player)
{
	player->AddAbility(m_GrantedAbility.release());
}

void AbilityPickup::OnCollision(Entity& other)
{
	if (other.GetType() == EObjectTypes::PLAYER)
	{
		Player* player = static_cast<Player*>(&other);
		GrantAbility(player);
		Destroy();
	}
}
