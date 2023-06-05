#pragma once
#include "Platform.h"

struct MovingPlatformConstructionData : public EntityConstructionData
{
	Vec2f StartPosition = { 0.0f, 0.0f };
	Vec2f EndPosition = { 0.0f, 0.0f };
	float MovementSpeed = 5.f;
};

class MovingPlatform : public Platform
{
	float m_Time = 0.f;
	Vec2f m_StartPosition = { 0.f, 0.f };
	Vec2f m_EndPosition = { 0.f, 0.f };
	float m_MovementSpeed = 0.f;

public:
	MovingPlatform(const MovingPlatformConstructionData& constructionData);


	virtual void Update(float deltaTime) override;

	virtual Vec2f CalculatePlacementPosition(const Platform* previousPlatform) override;

	virtual Vec2f GetWorldBoundsX() const override;
	virtual Vec2f GetWorldBoundsY() const override;

	virtual const EPlatformTypes GetPlatformType() const override
	{
		return EPlatformTypes::MOVING;
	}

	virtual bool CanSpawnEntities() const override
	{
		return false;
	}
};




