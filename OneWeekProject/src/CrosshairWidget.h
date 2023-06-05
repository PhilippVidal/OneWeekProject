#pragma once
#include "Widget.h"

class CrosshairWidget :public Widget
{
public:
	CrosshairWidget(const WidgetConstructionData& constructionData);

	virtual void Update(float deltaTime) override;
};

