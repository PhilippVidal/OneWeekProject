#include "Player.h"
#include "NotDoodleJump.h"
#include "Widget.h"
#include "Projectile.h"
#include "Ability.h"
#include "AbilityImmunity.h"
#include <iostream>

Player::Player(const PlayerConstructionData& constructionData)
	: Entity(constructionData)
{
	m_FireCooldownDuration = PLAYER_FIRE_COOLDOWN_DURATION;
	m_TimeSinceLastFire = m_FireCooldownDuration;
	m_SpriteObjectActiveAbility = GetGame()->GetSprite(constructionData.SpritePathActiveAbility);
}

bool Player::RemoveAbility(Ability* ability)
{
	if (!ability)
	{
		return false;
	}

	for (auto& ownedAbility : m_Abilities)
	{
		if (ownedAbility.get() == ability)
		{
			m_Abilities.back().swap(ownedAbility);
			m_Abilities.back()->OnRemoved(this);
			m_Abilities.pop_back();
			return true;
		}
	}
	
	return false;
}

bool Player::TryBuyAbility(EAbilityType ability)
{
	int cost = ABILITY_COST;
	if (m_CoinCount < cost)
	{
		return false;
	}

	m_CoinCount -= cost;
	AbilityConstructionData constrData;
	constrData.AssignedToPlayer = this;
	constrData.Duration = ABILITY_IMMUNITY_DURATION;
	AddAbility<AbilityImmunity>(constrData);
	return true;
}

void Player::UpdateAbilities(float deltaTime)
{
	for (auto& ability : m_Abilities)
	{
		if (ability)
		{
			ability->Update(deltaTime);
		}
	}
}

void Player::Die()
{
	m_IsDead = true;
	Destroy();
}

bool Player::CanFire() const
{
	return m_TimeSinceLastFire > m_FireCooldownDuration;
}

void Player::OnCollision(Entity& other)
{
	switch(other.GetType())
	{
		case EObjectTypes::PLATFORM:
			if (IsFallingOnEntity(other))
			{				
				Jump();
			}
			break;
		case EObjectTypes::ENEMY:
			if (IsFallingOnEntity(other))
			{
				Jump();
				other.Destroy();
				break;
			}

			if (CanDie())
			{
				Die();
			}

			break;
	}
}

void Player::Jump()
{
	SetVelocity(GetVelocity().x, m_JumpForce);
}

void Player::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	UpdateKeyInput(deltaTime);
	UpdateMouseInput(deltaTime);

	for (auto& ability : m_Abilities)
	{
		if (ability)
		{
			ability->Update(deltaTime);
		}
	}

	//Calculate gravity influence
	float gravityInfluence =  m_Gravity * deltaTime;

	//Update velocity
	Vec2f velocity = GetVelocity();
	velocity = { velocity.x, velocity.y + gravityInfluence };
	SetVelocity(velocity);

	//Calcualte new position based on velocity
	Vec2f newPosition = GetPosition() + velocity * deltaTime;

	newPosition.x = (newPosition.x > 0) ?
		fmodf(newPosition.x, GetGame()->GetWorldSize().x) 
		: newPosition.x + GetGame()->GetWorldSize().x;

	SetPosition(newPosition);

	if ((GetPosition().y + (GetSize().y * 0.5f)) < GetGame()->GetCameraPosition().y)
	{
		Die();
	}

	m_TimeSinceLastFire += deltaTime;
}

void Player::Fire()
{
	Fire(GetGame()->GetMousePositionWorld());
}

void Player::Fire(const Vec2f& position)
{
	m_TimeSinceLastFire = 0.f;
	float force = 50.f;

	ProjectileConstructionData projectileConstrData;
	projectileConstrData.SpritePath = SPRITE_PATH_PROJECTILE;
	projectileConstrData.ShouldUpdate = true;
	projectileConstrData.GameLayer = EGameLayers::PROJECTILES;
	projectileConstrData.ColliderType = EColliderTypes::CIRCLE;
	projectileConstrData.CollisionLayer = ECollisionLayers::PROJECTILE;
	projectileConstrData.Velocity = (position - GetPosition()).Normalized() * force;
	projectileConstrData.Position = GetPosition();
	projectileConstrData.FireOnConstruction = true;
	projectileConstrData.LifeTime = 6.0f;
	projectileConstrData.IsStatic = false;

	GetGame()->AddGameEntity<Projectile>(projectileConstrData);
}

bool Player::IsAboveEntity(const Entity& other) const
{
	Vec2f position = GetPosition();
	Vec2f size = GetSize();
	Vec2f positionOther = other.GetPosition();
	Vec2f sizeOther = other.GetSize();

	if (position.y > (positionOther.y + sizeOther.y * 0.5f))
	{
		float xMax = position.x + size.x * 0.5f;
		float xMin = position.x - size.x * 0.5f;

		bool left = xMax >= (positionOther.x - sizeOther.x * 0.5f);
		bool right = xMin <= (positionOther.x + sizeOther.x * 0.5f);

		return xMax >= (positionOther.x - sizeOther.x * 0.5f)
			&& xMin <= (positionOther.x + sizeOther.x * 0.5f);
	}

	return false;
}

bool Player::IsFallingOnEntity(const Entity& other) const
{
	return IsAboveEntity(other) && (GetVelocity().y <= 0);
}


void Player::UpdateKeyInput(float deltaTime) 
{
	Vec2f velocityChange;
	if (GetGame()->IsKeyPressed(EKeys::RIGHT))
	{
		velocityChange = velocityChange + Vec2f(m_MovementSpeed * deltaTime, 0.0f);
	}

	if (GetGame()->IsKeyPressed(EKeys::LEFT))
	{
		velocityChange = velocityChange + Vec2f(-m_MovementSpeed * deltaTime, 0.0f);
	}

	AddVelocity(velocityChange);
}

void Player::UpdateMouseInput(float deltaTime) 
{
	if (GetGame()->IsMouseButtonDown(EMouseButtons::LEFT))
	{
		if (CanFire())
		{
			Fire();
		}
	}

	if (GetGame()->IsMouseButtonDown(EMouseButtons::RIGHT))
	{
		TryBuyAbility(EAbilityType::IMMUNITY);
	}
}


void Player::Draw() const
{
	if (HasActiveAbility() && m_SpriteObjectActiveAbility)
	{
		GetGame()->DrawWorld(m_SpriteObjectActiveAbility, GetPosition(), GetSize() * 2);
	}

	Entity::Draw();
}

