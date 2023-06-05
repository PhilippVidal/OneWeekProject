#pragma once
#include "Entity.h"

class Coin : public Entity
{
public:

	Coin(const EntityConstructionData& constructionData);

protected:

	virtual void OnCollision(Entity& other) override;
};

