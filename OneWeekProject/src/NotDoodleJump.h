#pragma once
#include "Game.h"

//Convenient way of addressing game settings due to time constraints
//For a bigger/actual game this would obviously be done differently
//For example using a config that is loaded from a file
#define SPAWN_CHANCE_ABILITY 0.05f
#define SPAWN_CHANCE_COINS 0.65f
#define SPAWN_CHANCE_MOVINGPLATFORM 0.1f
#define SPAWN_CHANCE_BLACKHOLE 0.15f
#define PLAYER_FIRE_COOLDOWN_DURATION 5.0f
#define BLACKHOLE_FORCE 80.f
#define BLACKHOLE_MIN_GAP 500.f
#define SPAWN_MIN_PLATFORMS_BLACKHOLE 15
#define SPAWN_MIN_PLATFORMS_ENEMIES 10
#define SPAWN_MIN_PLATFORMS_ABILITIES 5
#define SPAWN_MIN_PLATFORMS_COINS 1
#define ABILITY_IMMUNITY_DURATION 20.f
#define ABILITY_COST 20

#define MOVING_PLATFORM_SPEED 0.5f

#define DEFAULT_SCREEN_WIDTH 1200 
#define DEFAULT_SCREEN_HEIGHT 1000

#define MIN_PLATFORM_GAP 4.0f
#define MAX_PLATFORM_GAP 50.0f
#define SPAWN_CHANCE_ENEMY 1.f

#define ENTITY_GENERATION_THRESHOLD 

#define SPRITE_PATH_PLAYER		"data/player.png"
#define SPRITE_PATH_PLATFORM	"data/platform.png"
#define SPRITE_PATH_BACKGROUND	"data/background.png"
#define SPRITE_PATH_ENEMY		"data/enemy.png"
#define SPRITE_PATH_ENEMY_2		"data/enemy_2.png"
#define SPRITE_PATH_CROSSHAIR	"data/crosshair.png"
#define SPRITE_PATH_PROJECTILE	"data/projectile.png"
#define SPRITE_PATH_GROUND		"data/ground.png"
#define SPRITE_PATH_ABILITY		"data/ability_immunity.png"
#define SPRITE_PATH_COIN		"data/coin.png"
#define SPRITE_PATH_SCORE_DIGIT_BASE	"data/digit_%i.png"
#define SPRITE_PATH_SCORE_DISTANCE		"data/score_distance.png"
#define SPRITE_PATH_SCORE_PLATFORMS		"data/score_platforms.png"
#define SPRITE_PATH_SCORE_COINS			"data/score_coins.png"
#define SPRITE_PATH_BLACKHOLE_OUTER		"data/blackhole_outer.png"
#define SPRITE_PATH_BLACKHOLE_CENTER	"data/blackhole.png"
#define SPRITE_PATH_PLAYER_ACTIVE_ABILITY "data/player_ability.png"

class Platform;
class Enemy;
class AbilityPickup;
class Blackhole;
class Coin;

enum class EObjectTypes : unsigned char;
enum class EPlatformTypes : unsigned char;

class NotDoodleJump : public Game
{
	enum class EPlatformSpawnables : unsigned char
	{
		ENEMIES,
		ABILITIES,
		COINS,
		COUNT
	};

	std::array<float, static_cast<std::underlying_type_t<EPlatformSpawnables>>(EPlatformSpawnables::COUNT)> m_SpawnProbabilities = {
		SPAWN_CHANCE_ENEMY,
		SPAWN_CHANCE_ABILITY,
		SPAWN_CHANCE_COINS
	};

	std::array<int, static_cast<std::underlying_type_t<EPlatformSpawnables>>(EPlatformSpawnables::COUNT)> m_SpawnablesMinPlatformsPassed = {
		SPAWN_MIN_PLATFORMS_ENEMIES,
		SPAWN_MIN_PLATFORMS_ABILITIES,
		SPAWN_MIN_PLATFORMS_COINS
	};


	Platform* m_HighestPlatform = nullptr;

	float m_HighestBlackholeY = 0.f;
	float m_ScoreDistance = 0.f;
	int m_ScorePlatformsPassed = 0.f;
	int m_ScoreCoinsCollected = 0.f;

	float m_GeneratedHeight = 0.0f;

	//Game Settings 
	float m_MaxPlatformGap = MAX_PLATFORM_GAP;
	float m_MinPlatformGap = MIN_PLATFORM_GAP;
	float m_SpawnChanceEnemy = SPAWN_CHANCE_ENEMY;
	float m_SpawnChanceAbility = SPAWN_CHANCE_ABILITY;
	float m_SpawnChanceCoins = SPAWN_CHANCE_ABILITY;
	float m_SpawnChanceMovingPlatform = SPAWN_CHANCE_MOVINGPLATFORM;
	float m_SpawnChanceBlackhole = SPAWN_CHANCE_BLACKHOLE;

	float m_BlackholeForce = BLACKHOLE_FORCE;
	float m_BlackholeMinGap = BLACKHOLE_MIN_GAP;

	class ScoreWidget* m_ScoreWidgetDistanceTraveled = nullptr;
	class ScoreWidget* m_ScoreWidgetPlatformsPassed = nullptr;
	class ScoreWidget* m_ScoreWidgetCointsCollected = nullptr;



public:

	NotDoodleJump(Framework* framework);

	void AddPassedPlatform()
	{
		m_ScorePlatformsPassed++;
	}

	virtual void PostInit() override;

protected:


	void UpdateScore();


	Platform* SpawnPlatform(EPlatformTypes platformType);
	Enemy* SpawnEnemy(Platform* platform);
	Coin* SpawnCoin(Platform* platform);
	AbilityPickup* SpawnAbility(Platform* platform);
	Vec2f FitEntityOnPlatform(Entity* entity, Platform* platform) const;

	Blackhole* SpawnBlackhole(float minHeight, float maxHeight);

	void CreateMoreEntities(float targetHeight);

	bool GetSpawnType(EPlatformSpawnables& outSpawnType) const;

	virtual void LateUpdate(float deltaTime) override;

	virtual void OnCameraMoved(const Vec2f& delta) override;

	virtual PixelCoords CalculateBaseWindowResolution() const override;

public: 

	void SetCoinsCollected(int coins)
	{
		m_ScoreCoinsCollected = coins;
	}

	int GetCoinsCollected() const
	{
		return m_ScoreCoinsCollected;

	}
	void SetMinPlatformGap(float value)
	{
		m_MinPlatformGap = value;
	}

	float GetMinPlatformGap() const
	{
		return m_MinPlatformGap;
	}

	void SetMaxPlatformGap(float value)
	{
		m_MaxPlatformGap = value;
	}

	float GetMaxPlatformGap() const
	{
		return m_MaxPlatformGap;
	}
};

