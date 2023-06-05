#pragma once
#include "Entity.h"

struct BlackholeConstructionData : public EntityConstructionData
{
	std::string SpritePathOuter = "";
	float Force = 10;
};

class Blackhole : public Entity
{
	float m_Radius = 5;
	float m_Force = 10;
	SpriteObject* m_SpriteObjectOuter = nullptr;

public:
	Blackhole(const BlackholeConstructionData& constructionData);

	virtual void Update(float deltaTime) override;

	void SetOuterSprite(SpriteObject* spriteObject)
	{
		m_SpriteObjectOuter = spriteObject;
	}

	SpriteObject* GetOuterSprite() const
	{
		return m_SpriteObjectOuter;
	}

	virtual void Draw() const override;

protected:

	virtual void OnCollision(Entity& other) override;
};

