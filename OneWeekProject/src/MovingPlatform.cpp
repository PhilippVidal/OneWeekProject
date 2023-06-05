#include "MovingPlatform.h"
#include "Math.h"

MovingPlatform::MovingPlatform(const MovingPlatformConstructionData& constructionData)
	: Platform(constructionData)
{
	m_Time = 0.0f;
	m_StartPosition = constructionData.StartPosition;
	m_EndPosition = constructionData.EndPosition;
	m_MovementSpeed = constructionData.MovementSpeed;
	SetPosition(m_StartPosition);
}

void MovingPlatform::Update(float deltaTime)
{
	Platform::Update(deltaTime);

	m_Time += m_MovementSpeed * deltaTime;
	float t = (sinf(Math::Lerp(-HALF_PI, HALF_PI, m_Time)) + 1.0f ) * 0.5f;;
	Vec2f newPosition = Math::Lerp(m_StartPosition, m_EndPosition, t);

	SetPosition(Math::Lerp(m_StartPosition, m_EndPosition, t));
}

Vec2f MovingPlatform::CalculatePlacementPosition(const Platform* previousPlatform)
{
	Vec2f platformSize = GetSize();
	Vec2f randomPositionRangeX;
	Vec2f randomPositionRangeY;
	CalculateRandomPositionRange(previousPlatform, platformSize, randomPositionRangeX, randomPositionRangeY);


	Vec2f halfPlatformSize = platformSize * 0.5f;

	float randomYValue = (randomPositionRangeY.max - randomPositionRangeY.min) - platformSize.y;
	randomYValue = Math::GetRandomFloat(0.f, (randomPositionRangeY.max - randomPositionRangeY.min) - platformSize.y);
	randomYValue += randomPositionRangeY.min + halfPlatformSize.y;

	m_StartPosition = {
		randomPositionRangeX.min + halfPlatformSize.x,
		randomYValue
	};

	m_EndPosition = {
		randomPositionRangeX.max - halfPlatformSize.x,
		randomYValue
	};

	return m_StartPosition;
}

Vec2f MovingPlatform::GetWorldBoundsX() const
{
	Vec2f bounds = Platform::GetWorldBoundsX();

	Vec2f movementBoundsX = {
		fminf(m_StartPosition.x, m_EndPosition.x),
		fmaxf(m_StartPosition.x, m_EndPosition.x)
	};


	return {
		fminf(bounds.min, movementBoundsX.min),
		fmaxf(bounds.max, movementBoundsX.max),
	};
}

Vec2f MovingPlatform::GetWorldBoundsY() const
{
	Vec2f bounds = Platform::GetWorldBoundsY();

	Vec2f movementBoundsY = {
		fminf(m_StartPosition.y, m_EndPosition.y),
		fmaxf(m_StartPosition.y, m_EndPosition.y)
	};

	return {
		fminf(bounds.min, movementBoundsY.min),
		fmaxf(bounds.max, movementBoundsY.max),
	};
}




