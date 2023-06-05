#pragma once
#include "Object.h"

struct WidgetConstructionData : public ObjectConstructionData
{
	PixelCoords ScreenPosition = { 0, 0 };
	Vec2f ScreenScale = { 1.f, 1.f };
	int RenderPriority = 0;
};

class Widget : public Object
{
	PixelCoords m_ScreenPosition = { 0, 0 };
	PixelCoords m_ScreenSize = { 1, 1 };
	Vec2f m_ScreenScale = { 1.f, 1.f };
	int m_RenderPriority = 0;

public:

	Widget(const WidgetConstructionData& constructionData);

	virtual void Draw() const override;

	virtual const EObjectTypes GetType() const override
	{
		return EObjectTypes::UI_WIDGET;
	}

	virtual PixelCoords GetScreenSize() const;

	void SetScreenPosition(PixelCoords screenPosition)
	{
		m_ScreenPosition = screenPosition;
	}

	PixelCoords GetScreenPosition() const
	{
		return m_ScreenPosition;
	}

	void SetScreenScale(Vec2f scale)
	{
		m_ScreenScale = scale;
	}

	Vec2f GetScreenScale() const
	{
		return m_ScreenScale;
	}

	int GetRenderPriority() const
	{
		return m_RenderPriority;
	}

};

