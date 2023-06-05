#pragma once
#include "Entity.h"

enum class EAbilityType : unsigned char;
class Ability;


struct PlayerConstructionData : EntityConstructionData
{
	std::string SpritePathActiveAbility = "";
};

class Player : public Entity
{

	SpriteObject* m_SpriteObjectActiveAbility = nullptr;

	float m_MovementSpeed = 50;

	float m_Gravity = -80.f;
	float m_JumpForce = 110.f;
	
	bool m_CanDie = true;

	int m_CoinCount = 0;

	float m_TimeSinceLastFire = 0.f;
	float m_FireCooldownDuration = 5.f;

	bool m_IsDead = false;

	std::vector<std::unique_ptr<Ability>> m_Abilities;


public:
	Player(const PlayerConstructionData& constructionData);

	void SetCanDie(bool value)
	{
		m_CanDie = value;
	}

	bool CanDie() const
	{
		return m_CanDie;
	}

	bool IsDead() const
	{
		return m_IsDead;
	}

	bool HasActiveAbility() const
	{
		return m_Abilities.size() != 0;
	}

	template<class T, class TypeArg>
	void AddAbility(const TypeArg& abilityData)
	{
		AddAbility(std::make_unique<T>(abilityData));		
	}

	template<class T>
	void AddAbility(T* ability)
	{
		AddAbility(std::unique_ptr<T>(ability));		
	}

	template<class T>
	void AddAbility(std::unique_ptr<T>&& abilityUniquePtr)
	{
		std::unique_ptr<T> grantedAbility = std::move(abilityUniquePtr);
		if (grantedAbility)
		{
			for (auto& ownedAbility : m_Abilities)
			{
				if (ownedAbility && ownedAbility->GetType() == grantedAbility->GetType())
				{
					ownedAbility->RefreshLifeTime();

					//Granted ability is being destroyed automatically via unique pointer leaving scope
					return;
				}
			}

			m_Abilities.push_back(std::move(grantedAbility));
			m_Abilities.back()->OnAdded(this);
		}
	}

	bool RemoveAbility(Ability* ability);

	bool TryBuyAbility(EAbilityType ability);


	void UpdateAbilities(float deltaTime);

	void Die();

	void AddCoins(int amount) 
	{
		m_CoinCount += amount;
	}

	void RemoveCoins(int amount)
	{
		m_CoinCount -= fminf(m_CoinCount, amount);
	}

	int GetCoinCount() const
	{
		return m_CoinCount;
	}

	virtual bool CanFire() const;

	virtual void OnCollision(Entity& other) override;

	void Jump(); 

	virtual void Update(float deltaTime) override;

	void Fire();
	void Fire(const Vec2f& position);

	virtual const EObjectTypes GetType() const override
	{
		return EObjectTypes::PLAYER;
	}

	virtual bool IsAboveEntity(const Entity& other) const;

	virtual bool IsFallingOnEntity(const Entity& other) const;

	virtual void UpdateKeyInput(float deltaTime);
	virtual void UpdateMouseInput(float deltaTime);

	virtual void Draw() const override;
};

