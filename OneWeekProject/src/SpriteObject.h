#pragma once
#include "Vec2.h"
#include <string>
#include <memory>

class Sprite;

class SpriteObject
{
	Sprite* m_Sprite = nullptr;
	PixelCoords m_OriginalSize = { 1, 1 };

public:
	SpriteObject() = delete;
	SpriteObject(const std::string& spritePath);
	virtual ~SpriteObject();

	PixelCoords GetOriginalSize() const 
	{
		return m_OriginalSize;
	}

	Sprite* GetSprite() const
	{
		return m_Sprite;
	}
};

