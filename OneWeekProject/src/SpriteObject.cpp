#include "SpriteObject.h"
#include "Framework.h"

SpriteObject::SpriteObject(const std::string& spritePath)
{
	m_Sprite = CreateSprite(spritePath.c_str());
	GetSpriteSize(m_Sprite, m_OriginalSize.x, m_OriginalSize.y);
}

SpriteObject::~SpriteObject()
{
	DeleteSprite(m_Sprite);
}
