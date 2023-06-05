#include "CrosshairWidget.h"
#include "Game.h"

CrosshairWidget::CrosshairWidget(const WidgetConstructionData& constructionData)
	: Widget(constructionData)
{

}

void CrosshairWidget::Update(float deltaTime)
{
	Widget::Update(deltaTime);
	SetScreenPosition(GetGame()->GetMousePosition());
}
