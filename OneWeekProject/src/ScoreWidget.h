#pragma once
#include "Widget.h"
#include <array>

struct ScoreWidgetConstructionData : public WidgetConstructionData
{
	int DigitAmount = 5;
	std::string DigitBasePath = "";
};

class ScoreWidget : public Widget
{
	struct DigitInfo
	{
		PixelCoords PositionOffset = { 0, 0 };
		unsigned char Digit = 0;
	};

	int m_DigitAmount = 5;

	typedef std::array<SpriteObject*, 10> DigitSprites;
	DigitSprites m_DigitSprites;

	PixelCoords m_DigitSize = { 0,0 };

	unsigned int m_Score = 0;

	PixelCoords m_RequiredSize = { 0, 0 };

	unsigned char m_ScoreDigitCount = 0;

	std::vector<DigitInfo> m_ScoreDigits;

	PixelCoords m_ScoreNameOffset = { 0, 0 };

public:
	ScoreWidget(const ScoreWidgetConstructionData& constructionData);

	void UpdateScore(unsigned int score);

	SpriteObject* RetrieveDigitSprite(unsigned char digit) const;

	virtual PixelCoords GetScreenSize() const override;

	void UpdateRequiredSize();

	virtual void Draw() const override;
 
	const DigitSprites& GetDigitSprites() const
	{
		return m_DigitSprites;
	}

	void SetDigitAmount(unsigned int amount) 
	{
		m_DigitAmount = amount;
		m_ScoreDigits.resize(m_DigitAmount);
	}

	unsigned int GetDigitAmount() const
	{
		return m_DigitAmount;
	}

	PixelCoords GetRequiredSize() const
	{
		return m_RequiredSize;
	}

	void SetScore(unsigned int score)
	{
		m_Score = score;
	}

	unsigned int GetScore() const
	{
		return m_Score;
	}
protected:

	void UpdateScoreDigits();

	void UpdatePositioning();

};

