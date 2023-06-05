#include "ScoreWidget.h"
#include "Game.h"
#include "SpriteObject.h"

ScoreWidget::ScoreWidget(const ScoreWidgetConstructionData& constructionData)
	: Widget(constructionData)
{	
	int size = constructionData.DigitBasePath.size();
	if (size > 0)
	{
		char* buffer = new char[size];
		const char* format = constructionData.DigitBasePath.c_str();
		for (int i = 0; i < 10; i++)
		{
			snprintf(buffer, size, format, i);
			m_DigitSprites[i] = GetGame()->GetSprite(buffer);
		}

		delete[] buffer;
		
	}

	SetDigitAmount(constructionData.DigitAmount);

	UpdateRequiredSize();
	UpdatePositioning();
}

void ScoreWidget::UpdateScore(unsigned int score)
{
	m_Score = score;
	UpdateScoreDigits();
}

SpriteObject* ScoreWidget::RetrieveDigitSprite(unsigned char digit) const
{
	if (m_DigitSprites.size() < 10)
	{
		return nullptr;
	}

	if (digit > 9)
	{
		digit = 9;
	}

	return m_DigitSprites[digit];
}

PixelCoords ScoreWidget::GetScreenSize() const
{
	return GetRequiredSize();
}

void ScoreWidget::UpdateRequiredSize()
{
	m_DigitSize = GetGame()->GetSpriteGameScreenSize(GetDigitSprites().at(0), GetScreenScale());
	Vec2f baseSpriteSize;
	if (SetDefaultSprite())
	{
		baseSpriteSize = GetGame()->GetSpriteGameScreenSize(SetDefaultSprite(), GetScreenScale());
	}

	m_RequiredSize = {
		baseSpriteSize.x + GetDigitAmount() * m_DigitSize.x,
		(baseSpriteSize.y > m_DigitSize.y) ? baseSpriteSize.y : m_DigitSize.y
	};
}

void ScoreWidget::Draw() const
{
	PixelCoords screenPosition = GetScreenPosition();
	SpriteObject* digitSprite;
	for (int i = 0; i < m_ScoreDigitCount; i++)
	{
		DigitInfo digitInfo = m_ScoreDigits[i];
		digitSprite = RetrieveDigitSprite(digitInfo.Digit);
		GetGame()->DrawScreen(digitSprite, screenPosition + digitInfo.PositionOffset, GetGame()->GetSpriteGameScreenSize(digitSprite, GetScreenScale()));
	}

	if (SetDefaultSprite())
	{
		GetGame()->DrawScreen(SetDefaultSprite(), screenPosition + m_ScoreNameOffset, GetGame()->GetSpriteGameScreenSize(SetDefaultSprite(), GetScreenScale()));
	}
}

void ScoreWidget::UpdateScoreDigits()
{
	unsigned int remainingNumber = GetScore();

	int i = 0;

	while (i < m_DigitAmount)
	{
		unsigned char digit = remainingNumber % 10;
		remainingNumber /= 10;

		m_ScoreDigits[i].Digit = digit;
		i++;
	}

	m_ScoreDigitCount = i;
}

void ScoreWidget::UpdatePositioning()
{
	PixelCoords positionOffset;
	positionOffset.y = 0;
	int digitWidth = m_DigitSize.x;
	int digitHalfWidth = digitWidth / 2;
	float halfRequiredWidth = GetRequiredSize().x / 2;

	for (size_t i = 0; i < m_DigitAmount; i++)
	{
		positionOffset.x = halfRequiredWidth - digitHalfWidth - i * digitWidth;
		m_ScoreDigits[i].PositionOffset = positionOffset;
	}

	if (SetDefaultSprite())
	{
		float spriteWidth = GetGame()->GetSpriteGameScreenSize(SetDefaultSprite(), GetScreenScale()).x;
		positionOffset.x = -halfRequiredWidth + spriteWidth * 0.5f - digitWidth;
		m_ScoreNameOffset = positionOffset;
	}
}
