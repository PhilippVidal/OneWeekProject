#include "Coin.h"
#include "Player.h"

Coin::Coin(const EntityConstructionData& constructionData)
	:Entity(constructionData)
{

}

void Coin::OnCollision(Entity& other)
{
	if (other.GetType() == EObjectTypes::PLAYER)
	{
		if (Player* player = static_cast<Player*>(&other))
		{
			player->AddCoins(1);
		}

		Destroy();
	}
}
