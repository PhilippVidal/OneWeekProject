#include "Object.h"
#include "Game.h"

Object::Object(const ObjectConstructionData& constructionData)
{
	SetDefaultSprite(GetGame()->GetSprite(constructionData.SpritePath));
	EnableUpdates(constructionData.ShouldUpdate);
	SetIsVisible(true);
}

void Object::Draw() const
{
	
}

void Object::Destroy()
{
	SetMarkedForDeletion(true);
	SetIsVisible(false);
}

void Object::Update(float deltaTime)
{

}

