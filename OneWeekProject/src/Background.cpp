#include "Background.h"
#include "Game.h"

Background::Background(const EntityConstructionData& constructionData)
	: Entity(constructionData)
{
	if (SetDefaultSprite())
	{
		float worldSize = GetGame()->GetWorldSize().y;
		float size = GetSize().y;
		int amount = ceilf(worldSize / size) + 1.0f;
		m_BackgroundPositions.resize(amount);
	}
}

void Background::Update(float deltaTime)
{
	Object::Update(deltaTime);

	Vec2f position = GetPosition();
	Vec2f size = GetSize();

	Vec2f cameraPosition = GetGame()->GetCameraPosition();
	Vec2f worldSize = GetGame()->GetWorldSize();

	float startOffset = cameraPosition.y - fmodf(cameraPosition.y, size.y) + 0.5f * size.y;
	for (int i = 0; i < m_BackgroundPositions.size(); i++)
	{
		m_BackgroundPositions[i] = Vec2f(worldSize.x * 0.5f + cameraPosition.x, startOffset + i * size.y * 0.99f);
	}	
}

void Background::Draw() const
{
	Vec2f size = GetSize();
	SpriteObject* spriteObject = SetDefaultSprite();
	for (const Vec2f& position : m_BackgroundPositions)
	{
		GetGame()->DrawWorld(spriteObject, position, size, true);
	}
}
