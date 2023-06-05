#pragma once
#include "Entity.h"

class AbilityPickup : public Entity
{
	std::unique_ptr<class Ability> m_GrantedAbility = nullptr;

public:
	AbilityPickup(const EntityConstructionData& constructionData);

	void GrantAbility(class Player* player);

	virtual void OnCollision(Entity& other) override;

	template<class T, class TypeArgs>
	void SetGrantedAbility(const TypeArgs& abilityData)
	{
		m_GrantedAbility = std::make_unique<T>(abilityData);
	}
};

