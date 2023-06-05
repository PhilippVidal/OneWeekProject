#pragma once

enum class EAbilityType : unsigned char
{
	BASE,
	IMMUNITY,
	COUNT
};

struct AbilityConstructionData
{
	class Player* AssignedToPlayer = nullptr;
	float Duration = 20.f;
};

class Ability
{
	float m_RemainingDuration = 0.f;
	float m_Duration = 20.f;
	class Player* m_Player = nullptr;

	
public:
	Ability(const AbilityConstructionData& constructionData);
	virtual ~Ability();

	virtual void OnAdded(class Player* player);
	virtual void OnRemoved(class Player* player);

	void RefreshLifeTime();

	void Remove();

	virtual void Update(float deltaTime);

	virtual const EAbilityType GetType() const
	{
		return EAbilityType::BASE;
	}
};

