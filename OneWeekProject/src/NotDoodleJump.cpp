#include "NotDoodleJump.h"

#include "Entity.h"
#include "Background.h"
#include "ScoreWidget.h"
#include "Platform.h"
#include "Enemy.h"
#include "Player.h"
#include "CrosshairWidget.h"
#include "AbilityPickup.h"
#include "AbilityImmunity.h"
#include "MovingPlatform.h"
#include "Blackhole.h"
#include "Coin.h"


NotDoodleJump::NotDoodleJump(Framework* framework)
	: Game(framework)
{

}

void NotDoodleJump::UpdateScore()
{
	if (!GetPlayer())
	{
		return;
	}

	Player* player = static_cast<Player*>(GetPlayer());
	m_ScoreCoinsCollected = player->GetCoinCount();
	float playerY = player->GetPosition().y;
	if (m_ScoreDistance < playerY)
	{
		m_ScoreDistance = playerY;
	}

	if (m_ScoreWidgetDistanceTraveled)
	{
		m_ScoreWidgetDistanceTraveled->UpdateScore(m_ScoreDistance);
	}

	if (m_ScoreWidgetPlatformsPassed)
	{
		m_ScoreWidgetPlatformsPassed->UpdateScore(m_ScorePlatformsPassed);
	}

	if (m_ScoreWidgetCointsCollected)
	{
		m_ScoreWidgetCointsCollected->UpdateScore(m_ScoreCoinsCollected);
	}	
}

Platform* NotDoodleJump::SpawnPlatform(EPlatformTypes platformType)
{
	Platform* platform;
	switch (platformType)
	{
		case EPlatformTypes::MOVING:
		{
			MovingPlatformConstructionData platformConstrData;
			platformConstrData.SpritePath = SPRITE_PATH_PLATFORM;
			platformConstrData.ShouldUpdate = true;
			platformConstrData.IsStatic = true;
			platformConstrData.GameLayer = EGameLayers::ENTITIES;
			platformConstrData.ColliderType = EColliderTypes::AABB;
			platformConstrData.CollisionLayer = ECollisionLayers::PLATFORM;
			platformConstrData.MovementSpeed = MOVING_PLATFORM_SPEED;


			platform = AddGameEntity<MovingPlatform>(platformConstrData);

		}
		break;
		default:
		{
			EntityConstructionData platformConstrData;
			platformConstrData.SpritePath = SPRITE_PATH_PLATFORM;
			platformConstrData.ShouldUpdate = true;
			platformConstrData.IsStatic = true;
			platformConstrData.GameLayer = EGameLayers::ENTITIES;
			platformConstrData.ColliderType = EColliderTypes::AABB;
			platformConstrData.CollisionLayer = ECollisionLayers::PLATFORM;

			platform = AddGameEntity<Platform>(platformConstrData);
		}
	}

	if (!platform)
	{
		std::cout << "Failed to create platform" << "\n";
		return nullptr;
	}

	Vec2f spawnPosition = platform->CalculatePlacementPosition(m_HighestPlatform);
	platform->SetPosition(spawnPosition);

	m_HighestPlatform = platform;

	if (platform->CanSpawnEntities())
	{
		EPlatformSpawnables spawnableType;
		if (GetSpawnType(spawnableType))
		{
			Entity* spawnable = nullptr;
			switch (spawnableType)
			{
				case EPlatformSpawnables::ENEMIES:
						spawnable = SpawnEnemy(platform);
					break;
				case EPlatformSpawnables::COINS:
					spawnable = SpawnCoin(platform);
					break;
				case EPlatformSpawnables::ABILITIES:
					spawnable = SpawnAbility(platform);
					break;
			}

			if (spawnable)
			{
				spawnable->SetPosition(FitEntityOnPlatform(spawnable, platform));
				platform->SetSpawnedEntityBounds(spawnable);
			}
		}
	}

	return platform;
}

Enemy* NotDoodleJump::SpawnEnemy(Platform* platform)
{
	if (!platform)
	{
		return nullptr;
	}

	EntityConstructionData entityConstrData;
	entityConstrData.ShouldUpdate = false;
	entityConstrData.IsStatic = true;
	entityConstrData.GameLayer = EGameLayers::ENEMIES;
	entityConstrData.CollisionLayer = ECollisionLayers::ENEMY;

	if (rand() % 2)
	{
		entityConstrData.SpritePath = SPRITE_PATH_ENEMY;
		entityConstrData.ColliderType = EColliderTypes::CIRCLE;
	}
	else
	{
		entityConstrData.SpritePath = SPRITE_PATH_ENEMY_2;
		entityConstrData.ColliderType = EColliderTypes::AABB;
		entityConstrData.Scale = Vec2f(0.7f, 0.7f);
	}

	Enemy* enemy = AddGameEntity<Enemy>(entityConstrData);

	enemy->SetPosition(FitEntityOnPlatform(enemy, platform));

	return enemy;
}

Coin* NotDoodleJump::SpawnCoin(Platform* platform)
{
	if (!platform)
	{
		return nullptr;
	}

	EntityConstructionData entityConstrData;
	entityConstrData.SpritePath = SPRITE_PATH_COIN;
	entityConstrData.ShouldUpdate = false;
	entityConstrData.IsStatic = true;
	entityConstrData.GameLayer = EGameLayers::ENTITIES;
	entityConstrData.ColliderType = EColliderTypes::CIRCLE;
	entityConstrData.CollisionLayer = ECollisionLayers::ENTITY;
	entityConstrData.Scale = Vec2f(0.8f, 0.8f);

	Coin* coin = AddGameEntity<Coin>(entityConstrData);

	coin->SetPosition(FitEntityOnPlatform(coin, platform));

	return coin;
}

AbilityPickup* NotDoodleJump::SpawnAbility(Platform* platform)
{
	if (!platform)
	{
		return nullptr;
	}


	EntityConstructionData pickupConstrData;
	pickupConstrData.SpritePath = SPRITE_PATH_ABILITY;
	pickupConstrData.ShouldUpdate = false;
	pickupConstrData.IsStatic = true;
	pickupConstrData.GameLayer = EGameLayers::ENTITIES;
	pickupConstrData.ColliderType = EColliderTypes::AABB;
	pickupConstrData.CollisionLayer = ECollisionLayers::ENTITY;
	pickupConstrData.Scale = Vec2f(0.8f, 0.8f);
	AbilityPickup* pickup = AddGameEntity<AbilityPickup>(pickupConstrData);

	AbilityConstructionData abilityConstData;
	abilityConstData.AssignedToPlayer = nullptr;
	abilityConstData.Duration = ABILITY_IMMUNITY_DURATION;
	pickup->SetGrantedAbility<AbilityImmunity>(abilityConstData);

	return pickup;
}

Vec2f NotDoodleJump::FitEntityOnPlatform(Entity* entity, Platform* platform) const
{
	float platformHalfHeight = platform->GetSize().y * 0.5f;
	float halfSize = entity->GetSize().y * 0.5f;
	Vec2f platformPosition = platform->GetPosition();

	float y = platformPosition.y + platformHalfHeight + halfSize;

	return Vec2f(
		platformPosition.x,
		platformPosition.y + platformHalfHeight + halfSize
	);
}

Blackhole* NotDoodleJump::SpawnBlackhole(float minHeight, float maxHeight)
{
	BlackholeConstructionData constrData;
	constrData.SpritePath = SPRITE_PATH_BLACKHOLE_CENTER;
	constrData.SpritePathOuter = SPRITE_PATH_BLACKHOLE_OUTER;
	constrData.ShouldUpdate = true;
	constrData.IsStatic = true;
	constrData.GameLayer = EGameLayers::BLACKHOLES;
	constrData.ColliderType = EColliderTypes::AABB;
	constrData.CollisionLayer = ECollisionLayers::ENTITY;
	constrData.Force = m_BlackholeForce;
	constrData.Position = {
		Math::GetRandomFloat(0.f, GetGame()->GetWorldSize().x),
		Math::GetRandomFloat(minHeight, maxHeight)
	};

	Blackhole* blackhole = AddGameEntity<Blackhole>(constrData);
	if (blackhole)
	{
		m_HighestBlackholeY = blackhole->GetPosition().y;
	}

	return blackhole;
}

void NotDoodleJump::CreateMoreEntities(float targetHeight)
{
	bool wasBlackholeSpawned = false;
	while (m_GeneratedHeight <  targetHeight)
	{
		bool canSpawnMovingPlatform = m_GeneratedHeight > GetWorldSize().y;
		EPlatformTypes spawnedPlatformType = (canSpawnMovingPlatform && Math::GetRandomFloat(0.f, 1.0f) < m_SpawnChanceMovingPlatform) ?
			EPlatformTypes::MOVING : 
			EPlatformTypes::STATIC;

		Platform* platform = SpawnPlatform(spawnedPlatformType);

		m_GeneratedHeight = platform->GetPosition().y;

		bool isFarEnough = m_BlackholeMinGap < (m_GeneratedHeight - m_HighestBlackholeY);
		if (isFarEnough && (Math::GetRandomFloat(0.f, 1.0f) < m_SpawnChanceBlackhole))
		{
			SpawnBlackhole(m_GeneratedHeight, targetHeight);
			wasBlackholeSpawned = true;
		}
	}
}

bool NotDoodleJump::GetSpawnType(EPlatformSpawnables& outSpawnType) const
{
	float cumulativeProbability = 0.0f;
	float maxProbability = 0.f;

	for (int minPlatformsPassed : m_SpawnablesMinPlatformsPassed)
	{
		maxProbability += (m_ScorePlatformsPassed > minPlatformsPassed);
	}

	float rand = Math::GetRandomFloat(0.f, maxProbability);
	for (int i = 0; i < m_SpawnProbabilities.size(); i++)
	{
		if (m_ScorePlatformsPassed < m_SpawnablesMinPlatformsPassed[i])
		{
			continue;
		}

		cumulativeProbability += m_SpawnProbabilities[i];
		if (cumulativeProbability > rand)
		{
			outSpawnType = static_cast<EPlatformSpawnables>(i);
			return true;
		}
	}

	return false;
}

void NotDoodleJump::PostInit()
{
	Vec2f worldCenter = GetWorldSize() / 2.f;

	//Player
	PlayerConstructionData playerConstrData;
	playerConstrData.SpritePath = SPRITE_PATH_PLAYER;
	playerConstrData.SpritePathActiveAbility = SPRITE_PATH_PLAYER_ACTIVE_ABILITY;
	playerConstrData.ShouldUpdate = true;
	playerConstrData.IsStatic = false;
	playerConstrData.GameLayer = EGameLayers::PLAYER;
	playerConstrData.ColliderType = EColliderTypes::AABB;
	playerConstrData.CollisionLayer = ECollisionLayers::PLAYER;
	playerConstrData.Position = worldCenter;
	playerConstrData.Scale = Vec2f(0.8f, 0.8f);

	m_Player = AddGameEntity<Player>(playerConstrData);

	//Background
	{
		EntityConstructionData backgroundConstrData;
		backgroundConstrData.SpritePath = SPRITE_PATH_BACKGROUND;
		backgroundConstrData.ShouldUpdate = true;
		backgroundConstrData.IsStatic = true;
		backgroundConstrData.GameLayer = EGameLayers::BACKGROUND;
		backgroundConstrData.ColliderType = EColliderTypes::NONE;
		backgroundConstrData.CollisionLayer = ECollisionLayers::ENTITY;
		backgroundConstrData.Position = worldCenter;

		Background* background = AddGameEntity<Background>(backgroundConstrData);
	}
		
	//Spawn platforms, abilities, enemies, etc.
	CreateMoreEntities(GetCameraPosition().y + GetWorldSize().y * 1.5f);

	//Crosshair
	{
		WidgetConstructionData crosshairConstrData;
		crosshairConstrData.SpritePath = SPRITE_PATH_CROSSHAIR;
		crosshairConstrData.ShouldUpdate = true;
		crosshairConstrData.RenderPriority = static_cast<int>(EWidgetLayers::CROSSHAIR);
		crosshairConstrData.ScreenPosition = GetGame()->GetMousePosition();
		crosshairConstrData.ScreenScale = Vec2f(0.7f, 0.7f);
		AddWidget<CrosshairWidget>(crosshairConstrData);
	}

	//Score Widgets
	{
		//Score - distance traveled
		ScoreWidgetConstructionData scoreConstrData;
		scoreConstrData.SpritePath = SPRITE_PATH_SCORE_DISTANCE;
		scoreConstrData.DigitAmount = 5;
		scoreConstrData.DigitBasePath = SPRITE_PATH_SCORE_DIGIT_BASE;
		scoreConstrData.RenderPriority = static_cast<int>(EWidgetLayers::SCORE);

		m_ScoreWidgetDistanceTraveled = AddWidget<ScoreWidget>(scoreConstrData);

		m_ScoreWidgetDistanceTraveled->UpdateScore(0);

		PixelCoords gameScreenResolution = GetGameScreenResolution();
		PixelCoords topRightCorner = PixelCoords(gameScreenResolution.x, 0);
		PixelCoords distanceWidgetSize = m_ScoreWidgetDistanceTraveled->GetScreenSize();
		PixelCoords screenPosition = PixelCoords(topRightCorner.x - (distanceWidgetSize.x / 2), topRightCorner.y + (distanceWidgetSize.y / 2));
	
		m_ScoreWidgetDistanceTraveled->SetScreenPosition(screenPosition);
	 



		//Score - Platforms passed
		scoreConstrData.SpritePath = SPRITE_PATH_SCORE_PLATFORMS;
		scoreConstrData.DigitAmount = 4;

		m_ScoreWidgetPlatformsPassed = AddWidget<ScoreWidget>(scoreConstrData);

		m_ScoreWidgetPlatformsPassed->UpdateScore(0);

		PixelCoords platformsPassedWidgetSize = m_ScoreWidgetPlatformsPassed->GetScreenSize();
		screenPosition.x = topRightCorner.x - (platformsPassedWidgetSize.x / 2);
		screenPosition.y = topRightCorner.y + (platformsPassedWidgetSize.y / 2) + distanceWidgetSize.y;
		m_ScoreWidgetPlatformsPassed->SetScreenPosition(screenPosition);

		//Score - Coints collected
		scoreConstrData.SpritePath = SPRITE_PATH_SCORE_COINS;
		scoreConstrData.DigitAmount = 3;
		m_ScoreWidgetCointsCollected = AddWidget<ScoreWidget>(scoreConstrData);
	
		m_ScoreWidgetCointsCollected->UpdateScore(
			GetPlayer() ? 
			static_cast<Player*>(GetPlayer())->GetCoinCount() : 
			0
		);

		screenPosition.x = topRightCorner.x - (m_ScoreWidgetCointsCollected->GetScreenSize().x / 2);
		screenPosition.y = topRightCorner.y + (m_ScoreWidgetCointsCollected->GetScreenSize().y / 2) + distanceWidgetSize.y + platformsPassedWidgetSize.y;
		m_ScoreWidgetCointsCollected->SetScreenPosition(screenPosition);
	}

}

void NotDoodleJump::LateUpdate(float deltaTime)
{
	Game::LateUpdate(deltaTime);

	if (!GetPlayer())
	{
		return;
	}

	float playerPositionY = GetPlayer()->GetPosition().y;
	float thresholdHeight = GetWorldSize().y * 0.6f + GetCameraPosition().y;
	if (playerPositionY > thresholdHeight)
	{
		MoveCamera(Vec2f(0.f, playerPositionY - thresholdHeight));
	}

	UpdateScore();
}

void NotDoodleJump::OnCameraMoved(const Vec2f& delta)
{
	Game::OnCameraMoved(delta);

	if (!GetPlayer())
	{
		return;
	}

	float playerY = GetPlayer()->GetPosition().y;
	float distanceToScreenTop = (GetCameraPosition().y + GetWorldSize().y) - playerY;
	float distanceToGeneretedHeight = m_GeneratedHeight - playerY;
	if (GetPlayer() && (distanceToScreenTop < distanceToGeneretedHeight))
	{
		float targetHeight = GetCameraPosition().y + GetWorldSize().y * 1.5f;
		CreateMoreEntities(targetHeight);
	}
}

PixelCoords NotDoodleJump::CalculateBaseWindowResolution() const
{
	return PixelCoords(640, 1024);
}

