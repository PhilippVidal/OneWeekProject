#include "Widget.h"
#include "Game.h"

Widget::Widget(const WidgetConstructionData& constructionData)
    : Object(constructionData)
{
	m_RenderPriority = constructionData.RenderPriority;
	SetScreenPosition(constructionData.ScreenPosition);
	SetScreenScale(constructionData.ScreenScale);
}

void Widget::Draw() const
{
	GetGame()->DrawScreen(SetDefaultSprite(), GetScreenPosition(), GetScreenSize());
}

PixelCoords Widget::GetScreenSize() const
{
	if (!SetDefaultSprite())
	{
		return PixelCoords(0, 0);
	}

	return GetGame()->GetSpriteGameScreenSize(SetDefaultSprite(), m_ScreenScale);
}
