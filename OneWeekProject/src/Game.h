#pragma once

#include "GameFramework.h"
#include "ECollisionLayers.h"
#include "EGameLayers.h"
#include "Vec2.h"

#include "SpriteObject.h"
#include "Entity.h"
#include "Widget.h"

#include <vector>
#include <list>
#include <unordered_map>
#include <string>
#include <array>
#include <iostream>


#define GAME_GARBAGE_COLLECTION_INTERVAL 10.0f

class Game
{

protected:
	
	std::unordered_map<std::string, std::unique_ptr<SpriteObject>> m_LoadedSprites;

	//"Layers" are used to determine drawing order since the sprite rendering framework does not allow
	//rendering of sprites based on a given priority (i.e., render a sprite in front or behind other sprites)
	typedef std::vector<std::unique_ptr<Entity>> GameLayer;
	std::vector<std::unique_ptr<GameLayer>> m_GameEntities;

	//I'm using a list here because it makes it fairly easy to maintain the order of widgets
	//when adding/removing 
	//Due to the restrictions of the sprite rendering framework
	//preserving the order is important to draw them based on their priority,
	//so that if multiple widgets would occupy the same screen space they
	//will not draw over each other by accident/when it's not intended
	std::list<std::unique_ptr<Widget>> m_Widgets;

	
	float m_GarbageCollectionInterval = GAME_GARBAGE_COLLECTION_INTERVAL; 
	float m_TimeSinceLastGarbageCollection = 0.0f;

	class Framework* m_Framework = nullptr;

	Vec2f m_CameraPosition = {0.f, 0.f};

	PixelCoords m_GameWindowResolution = { 0, 0 };

	float m_PixelToWorldRatio = 1.f;

	Entity* m_Player = nullptr;

	bool m_WasInitSuccessful = false;

	float m_GameScreenScale = 1.f;

	const std::array<CollisionMask, static_cast<CollisionMask>(ECollisionLayers::COUNT)> m_CollisionLayerMasks = {
		0xffffffff,	//ENTITY		-> All
		0xffffffff,	//PLAYER		-> All
		0x00000012,	//ENEMY			-> Player & Projectiles
		0x00000002,	//PLATFORM		-> Player
		0x00000004,	//PROJECTILE	-> Enemies
		0x00000002	//ABILITIES		-> Player
	};

public:

	Game() = delete;
	Game(Framework* framework);
	virtual ~Game() {};
	

	template<class T>
	void RunOnGameEntities(T function, bool reverseOrder);

	template<class T, class TypeArgs>
	T* AddGameEntity(const TypeArgs& constrData)
	{
		std::unique_ptr<T> createdEntity = std::make_unique<T>(constrData);
		if (auto& gameLayer = m_GameEntities[createdEntity->GetGameLayerIndex()])
		{
			gameLayer->push_back(std::move(createdEntity));
			return static_cast<T*>(gameLayer->back().get());
		}

		return nullptr;
	}

	template<class T, class TypeArgs>
	T* AddWidget(const TypeArgs& constrData)
	{
		std::unique_ptr<T> createdWidget = std::make_unique<T>(constrData);

		auto iterator = m_Widgets.begin();
		while (iterator != m_Widgets.end())
		{
			auto& otherWidget = *iterator;
			if (otherWidget && otherWidget->GetRenderPriority() > createdWidget->GetRenderPriority())
			{
				auto insertIterator = m_Widgets.insert(iterator, std::move(createdWidget));
				return static_cast<T*>((*insertIterator).get());	
			}

			iterator++;
		}

		m_Widgets.push_back(std::move(createdWidget));
		return static_cast<T*>(m_Widgets.back().get());
	}

	void EvaluateCollision(Entity& entityA, Entity& entityB);

	void CollectGarbage();

	Entity* GetPlayer();

	void MoveCamera(Vec2f offset);

	virtual void Update(float deltaTime);

	Vec2f ScreenToWorld(PixelCoords screenPosition) const;

	PixelCoords WorldToScreen(Vec2f worldPosition) const;

	void DrawWorld(SpriteObject* spriteObject, Vec2f worldPosition, Vec2f worldSize, bool bAutoOffset = true) const;
	void DrawScreen(SpriteObject* spriteObject, PixelCoords screenPosition, PixelCoords size, bool bAutoOffset = true) const;

	SpriteObject* GetSprite(const std::string& path);


	Vec2f GetSpriteWorldSize(SpriteObject* spriteObject, Vec2f worldScale) const;
	PixelCoords GetSpriteGameScreenSize(SpriteObject* spriteObject, Vec2f scale) const;

	virtual bool Init();

	virtual void PostInit() {};

	void Restart();

protected:

	bool SetupGameLayers();

	virtual void UpdateEntities(float deltaTime);

	virtual void UpdateCollisions(float deltaTime);

	virtual void UpdateWidgets(float deltaTime);


	virtual void LateUpdate(float deltaTime);

	virtual void OnCameraMoved(const Vec2f& delta);

	virtual const float GetGameUnitsAlongX() const
	{
		return 100.f;
	}

public:
	
	virtual PixelCoords CalculateBaseWindowResolution() const;

	float GetGameWindowAspectRatio() const
	{
		return (m_GameWindowResolution.y > 0)
			? (m_GameWindowResolution.x / static_cast<float>(m_GameWindowResolution.y))
			: 1.f;	
	}

	void CalculatePixelToWorldRatio()
	{
		SetPixelToWorldRatio(GetGameUnitsAlongX() / m_GameWindowResolution.x);
	}

	bool WasInitSuccessful() const
	{
		return m_WasInitSuccessful;
	}

	CollisionMask GetCollisionMask(ECollisionLayers layer)
	{
		return m_CollisionLayerMasks[static_cast<CollisionMask>(layer)];
	}

	Vec2f GetWorldSize() const
	{
		return GetGameScreenResolution() * GetPixelToWorldRatio();
	}

	void SetPixelToWorldRatio(float ratio)
	{
		m_PixelToWorldRatio = ratio;
	}

	float GetPixelToWorldRatio() const
	{
		return m_PixelToWorldRatio;
	}

	void SetGameScreenScale(float scale)
	{
		m_GameScreenScale = scale;
	}

	float GetGameScreenScale() const
	{
		return m_GameScreenScale;
	}

	void SetGameScreenResolution(PixelCoords resolution)
	{
		m_GameWindowResolution = resolution;
		CalculatePixelToWorldRatio();
	}

	PixelCoords GetGameScreenResolution() const
	{
		return m_GameWindowResolution;
	}

	PixelCoords GetWindowScreenResolution() const
	{
		return m_Framework->GetScreenSize();
	}

	void SetCameraPosition(Vec2f position);

	Vec2f GetCameraPosition() const
	{
		return m_CameraPosition;
	}

	bool IsKeyDown(EKeys key) const
	{
		return m_Framework->IsKeyDown(key);
	}

	bool IsKeyReleased(EKeys key) const
	{
		return m_Framework->IsKeyReleased(key);
	}

	bool IsKeyPressed(EKeys key) const
	{
		return m_Framework->IsKeyPressed(key);
	}

	bool IsMouseButtonDown(EMouseButtons button) const
	{
		return m_Framework->IsMouseButtonDown(button);
	}

	bool IsMouseButtonReleased(EMouseButtons button) const
	{
		return m_Framework->IsMouseButtonReleased(button);
	}

	bool IsMouseButtonPressed(EMouseButtons button) const
	{
		return m_Framework->IsMouseButtonPressed(button);
	}

	PixelCoords GetMousePosition() const
	{
		return m_Framework->GetMousePositionGameWindow();
	}

	Vec2f GetMousePositionWorld() const
	{
		return ScreenToWorld(GetMousePosition());
	}


	Entity* GetPlayer() const 
	{
		return m_Player;
	}

	void SetPlayer(Entity* player)
	{
		m_Player = player;
	}

	virtual const char* GetTitle() const
	{
		return "Generic Game";
	}
};

template<class T>
inline void Game::RunOnGameEntities(T function, bool reverseOrder)
{
	for (auto& gameLayer : m_GameEntities)
	{
		if (!gameLayer)
		{
			std::cout << "Failed to setup game layers" << "\n";
			continue;
		}

		if (reverseOrder)
		{
			for (int i = gameLayer->size() - 1; i > 0; i--)
			{
				function(gameLayer->at(i).get(), gameLayer.get(), i);
			}

			continue;
		}

		for (int i = 0; i < gameLayer->size(); i++)
		{
			function(gameLayer->at(i).get(), gameLayer.get(), i);
		}
	}
}

extern std::unique_ptr<Framework> g_Framework;

Game* GetGame();

template<class T>
T* GetGame()
{
	return dynamic_cast<T*>(GetGame());
}