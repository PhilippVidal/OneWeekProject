#pragma once
#include "Entity.h"


struct ProjectileConstructionData : public EntityConstructionData
{
	float LifeTime = 10.0f;
	bool FireOnConstruction = true;
};

class Projectile :public Entity
{
	float m_LifeTime = 5.0f;
	float m_RemainingLifeTime = 5.0f;
	bool m_IsFired = false;

public:
	Projectile(const ProjectileConstructionData& constructionData);
	
	void Fire();

	void Reset();

	virtual void Update(float deltaTime) override;

	virtual void OnCollision(Entity& other) override;

	virtual const EObjectTypes GetType() const override
	{
		return EObjectTypes::PROJECTILE;
	}

	void ResetLifeTime();

	void SetLifeTime(float value)
	{
		m_LifeTime = value;
		ResetLifeTime();
	}

	float GetLifeTime() const
	{
		return m_LifeTime;
	}

	float GetRemainingLifeTime() const
	{
		return m_RemainingLifeTime;
	}

	bool IsFired() const
	{
		return m_IsFired;
	}

protected:

	void SetRemainingLifeTime(float value)
	{
		m_RemainingLifeTime = value;
	}
	
};

