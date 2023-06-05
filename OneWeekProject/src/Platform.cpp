#include "Platform.h"
#include "NotDoodleJump.h"


Platform::Platform(const EntityConstructionData& constructionData)
	: Entity(constructionData)
{
	
}

void Platform::CalculateRandomPositionRange(const Platform* previousPlatform, Vec2f size, Vec2f& randomPositionRangeX, Vec2f& randomPositionRangeY) const
{
	NotDoodleJump* game = GetGame<NotDoodleJump>();
	Vec2f worldSize = GetGame()->GetWorldSize();

	randomPositionRangeX = Vec2f(0.f, worldSize.x);
	randomPositionRangeY = Vec2f(game->GetMinPlatformGap(), game->GetMaxPlatformGap());

	if (!previousPlatform)
	{
		return;
	}

	Vec2f highestPlatformPosition = previousPlatform->GetPosition();

	randomPositionRangeY.min += highestPlatformPosition.y;
	randomPositionRangeY.max += highestPlatformPosition.y;

	Vec2f boundsX = previousPlatform->GetWorldBoundsX();

	//Can the platform fit left or right of the previous platform?
	float spaceLeft = boundsX.min;
	float spaceRight = worldSize.x - boundsX.max;
	unsigned int fitCase = (size.x < spaceLeft) | ((size.x < spaceRight) << 1);
	switch (fitCase)
	{
	case 0:
	{
		//Can not fit left or right -> platform will be completely above
		//use full x distance but adjust y
		Vec2f highestPlatformBoundsY = previousPlatform->GetWorldBoundsY();
		if (randomPositionRangeY.min < highestPlatformBoundsY.max)
		{
			randomPositionRangeY.min = highestPlatformBoundsY.max;

			if (randomPositionRangeY.max < randomPositionRangeY.min)
			{
				randomPositionRangeY.max = randomPositionRangeY.min;
			}
		}

	}
	break;
	case 1:
	{
		//Can fit left, but not right -> use only left distance
		randomPositionRangeX = { 0.f, boundsX.min };
	}
	break;
	case 2:
	{
		//Can fit right, but not left -> use only right distance
		randomPositionRangeX = { boundsX.min, worldSize.x };
	}
	break;
	case 3:
	{
		//Can fit either
		float max = previousPlatform->GetWorldBoundsY().max;
		if (randomPositionRangeY.min < max)
		{
			//Directly above is not possible, choose left or right randomly
			(rand() % 2) ?
				randomPositionRangeX = { boundsX.min, worldSize.x } :
				randomPositionRangeX = { 0.f, boundsX.min };

			break;
		}
	}
	break;
	}
}

Vec2f Platform::CalculatePlacementPosition(const Platform* previousPlatform)
{
	if (!previousPlatform)
	{
		if (Entity* player = GetGame()->GetPlayer())
		{
			Vec2f playerPosition = player->GetPosition();
			playerPosition.y -= player->GetSize().y * 2;
			return playerPosition;
		}

		Vec2f position = GetGame()->GetWorldSize();
		position.x /= 2;
		position.y /= 4;
		return position;
	}

	Vec2f platformSize = GetSize();
	Vec2f randomPositionRangeX;
	Vec2f randomPositionRangeY;
	CalculateRandomPositionRange(previousPlatform, platformSize, randomPositionRangeX, randomPositionRangeY);


	Vec2f randomValueRanges = {
		(randomPositionRangeX.max - randomPositionRangeX.min) - platformSize.x,
		(randomPositionRangeY.max - randomPositionRangeY.min) - platformSize.y
	};

	Vec2f randomValues = {
		Math::GetRandomFloat(0.f, randomValueRanges.x),
		Math::GetRandomFloat(0.f, randomValueRanges.y)
	};

	return {
		randomValues.x + randomPositionRangeX.min + platformSize.x * 0.5f,
		randomValues.y + randomPositionRangeY.min + platformSize.y * 0.5f
	};
}

void Platform::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	if (m_IsCountedInScore)
	{
		return;
	}

	Entity* player = GetGame()->GetPlayer();
	if (!player)
	{
		return;
	}

	float playerY = player->GetPosition().y;
	float positionY = GetPosition().y;

	if (positionY < playerY)
	{
		m_IsCountedInScore = true;
		static_cast<NotDoodleJump*>(GetGame())->AddPassedPlatform();
	}
}

void Platform::SetSpawnedEntityBounds(Entity* entity)
{
	if (entity)
	{
		m_HasSpawnedEntity = true;
		m_SpawnedEntityBoundsX = entity->GetWorldBoundsX();
		m_SpawnedEntityBoundsY = entity->GetWorldBoundsY();
	}
}

Vec2f Platform::GetWorldBoundsX() const
{
	Vec2f bounds = Entity::GetWorldBoundsX();

	if (m_HasSpawnedEntity)
	{
		
		Vec2f otherBounds = m_SpawnedEntityBoundsX;
		bounds.x = fminf(bounds.x, otherBounds.x);
		bounds.y = fmaxf(bounds.y, otherBounds.y);
	}

	return bounds;
}


Vec2f Platform::GetWorldBoundsY() const
{
	Vec2f bounds = Entity::GetWorldBoundsY();

	if (m_HasSpawnedEntity)
	{
		Vec2f otherBounds = m_SpawnedEntityBoundsY;
		bounds.x = fminf(bounds.x, otherBounds.x);
		bounds.y = fmaxf(bounds.y, otherBounds.y);
	}

	return bounds;
}
