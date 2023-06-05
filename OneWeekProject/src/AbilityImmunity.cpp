#include "AbilityImmunity.h"
#include "Player.h"

AbilityImmunity::AbilityImmunity(const AbilityConstructionData& constructionData)
	: Ability(constructionData)
{
}

void AbilityImmunity::OnAdded(Player* player)
{
	Ability::OnAdded(player);

	if (player)
	{
		player->SetCanDie(false);
	}
}

void AbilityImmunity::OnRemoved(Player* player)
{
	Ability::OnRemoved(player);

	if (player)
	{
		player->SetCanDie(true);
	}

}
