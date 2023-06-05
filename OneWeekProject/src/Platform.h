#pragma once
#include "Entity.h"

enum class EPlatformTypes : unsigned char
{
	STATIC,
	MOVING
};

class Platform : public Entity
{
	bool m_IsCountedInScore = false;
	bool m_HasSpawnedEntity = false;
	Vec2f m_SpawnedEntityBoundsX = { 0.f, 0.f };
	Vec2f m_SpawnedEntityBoundsY = { 0.f, 0.f };

public:

	Platform(const EntityConstructionData& constructionData);

	virtual void CalculateRandomPositionRange(const Platform* previousPlatform, Vec2f size, Vec2f& randomPositionRangeX, Vec2f& randomPositionRangeY) const;
	virtual Vec2f CalculatePlacementPosition(const Platform* previousPlatform);

	void SetSpawnedEntityBounds(Entity* entity);

	virtual void Update(float deltaTime) override;

	virtual Vec2f GetWorldBoundsX() const override;
	virtual Vec2f GetWorldBoundsY() const override;

	virtual const EObjectTypes GetType() const override
	{
		return EObjectTypes::PLATFORM;
	}

	virtual const EPlatformTypes GetPlatformType() const
	{
		return EPlatformTypes::STATIC;
	}

	virtual bool CanSpawnEntities() const 
	{
		return true;
	}
};

