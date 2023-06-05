#pragma once

#include "Vec2.h"
#include <vector>
#include <string>


enum class EObjectTypes : unsigned char
{
	OBJECT,
	UI_WIDGET,
	ENTITY,
	PLATFORM,
	PROJECTILE,
	ABILITY,
	PLAYER,
	ENEMY
};

enum class EWidgetLayers : unsigned char
{
	DEFAULT,
	CROSSHAIR,
	SCORE,
	COVER,
	COUNT
};

class SpriteObject;


struct ObjectConstructionData
{
	std::string SpritePath = "";
	bool ShouldUpdate = false;
};

class Object
{
	/* Default Sprite used to draw object */
	SpriteObject* m_SpriteObject = nullptr;

	bool m_ShouldUpdate = false; 

	bool m_IsMarkedForDeletion = false;

	bool m_IsVisible = false;

public:

	Object() = delete;

	Object(const ObjectConstructionData& constructionData);

	virtual ~Object() {};

	virtual void Draw() const;

	virtual void Update(float deltaTime);
	
	virtual void Destroy();


	virtual const EObjectTypes GetType() const
	{
		return EObjectTypes::OBJECT;
	}

	void SetIsVisible(bool value)
	{
		m_IsVisible = value;
	}

	bool IsVisible() const
	{
		return m_IsVisible;
	}


	void SetDefaultSprite(SpriteObject* sprite)
	{
		m_SpriteObject = sprite;
	}

	SpriteObject* SetDefaultSprite() const
	{
		return m_SpriteObject;
	}


	void EnableUpdates(bool value)
	{
		m_ShouldUpdate = value;
	}

	bool IsUpdatingEnabled() const
	{
		return m_ShouldUpdate;
	}


	void SetMarkedForDeletion(bool value)
	{
		m_IsMarkedForDeletion = value;
	}

	bool IsMarkedForDeletion() const
	{
		return m_IsMarkedForDeletion;
	}
};

