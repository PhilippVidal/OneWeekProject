#pragma once
#include "Entity.h"
#include <vector>

class Background : public Entity
{
	std::vector<Vec2f> m_BackgroundPositions;

public:
	Background(const EntityConstructionData& constructionData);

	virtual void Update(float deltaTime) override;

	virtual void Draw() const override;
};

