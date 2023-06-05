#pragma once
#include "Entity.h"

class Platform;

class Enemy :public Entity
{

public:
	Enemy(const EntityConstructionData& constructionData);

	virtual const EObjectTypes GetType() const override
	{
		return EObjectTypes::ENEMY;
	}
};

