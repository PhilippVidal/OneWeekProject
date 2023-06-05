#pragma once
#include "Ability.h"

class AbilityImmunity : public Ability
{
public:
	AbilityImmunity(const AbilityConstructionData& constructionData);

	virtual void OnAdded(Player* player) override;
	virtual void OnRemoved(Player* player) override;

	virtual const EAbilityType GetType() const override
	{
		return EAbilityType::IMMUNITY;
	}
};

