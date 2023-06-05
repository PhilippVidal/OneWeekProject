#include "Game.h"
#include "Entity.h"
#include "Widget.h"
#include "SpriteObject.h"
#include "Player.h"

Game::Game(Framework* framework)
{
	m_Framework = framework;
}

Entity* Game::GetPlayer()
{
	return m_Player;
}

void Game::MoveCamera(Vec2f offset)
{
	SetCameraPosition(GetCameraPosition() + offset);
}

void Game::EvaluateCollision(Entity& entityA, Entity& entityB)
{
	if (!entityA.IsCollisionEnabled() || !entityB.IsCollisionEnabled())
	{
		return;
	}

	if (!entityA.CanCollideWithOther(entityB) || !entityB.CanCollideWithOther(entityA))
	{
		return;
	}

	if (ColliderBase* collider = entityA.GetCollider())
	{
		collider->EvaluateCollision(&entityB);
	}
}

void Game::CollectGarbage()
{
	m_TimeSinceLastGarbageCollection = 0.0f;

	auto funcGarbageCollection = [this](Entity* entity, GameLayer* gameLayer, int index) {

		if (entity && !entity->IsMarkedForDeletion())
		{
			return;
		}

		gameLayer->back().swap(gameLayer->at(index));

		//Garbage collection for separate deleting in an ordered manner could be done here 
		//I simply let them be deleted directly through unique pointers here since it doesn't matter much
		//for a small game like this, deleting happens infrequently and not in significant enough quantities 
		//to cause any issues
		gameLayer->pop_back();
	};

	RunOnGameEntities(funcGarbageCollection, true);
}

bool Game::SetupGameLayers()
{
	//Setup the different game layers to place all entities in
	//If rough quantity is known allocation of sufficient memory could happen here beforehand
	//to prevent unnecessary reallocations of the vectors (and their potential performance issues) at run-time
	for (int i = 0; i < static_cast<int>(EGameLayers::COUNT); i++)
	{
		m_GameEntities.push_back(std::make_unique<GameLayer>());
	}

	return m_GameEntities.size() != 0;
}

void Game::UpdateEntities(float deltaTime)
{
	auto funcUpdateAndDraw = [deltaTime](Entity* entity, GameLayer* gameLayer, int index)
	{
		if (!entity)
		{
			return;
		}

		if (entity->IsUpdatingEnabled() && !entity->IsMarkedForDeletion())
		{
			entity->Update(deltaTime);
		}

		if (entity->IsVisible())
		{
			entity->Draw();
		}
	};

	RunOnGameEntities(funcUpdateAndDraw, false);
}

void Game::UpdateCollisions(float deltaTime)
{
	auto funcCollisionCheck = [this](Entity* entity, GameLayer* gameLayer, int index)
	{

		if (!entity || entity->IsStatic() || entity->IsMarkedForDeletion())
		{
			return;
		}

		auto funcCheckCollisionWith = [this, entity](Entity* other, GameLayer* gameLayer, int index)
		{
			this->EvaluateCollision(*entity, *other);
		};


		RunOnGameEntities(funcCheckCollisionWith, false);
	};


	RunOnGameEntities(funcCollisionCheck, false);
}

Vec2f Game::GetSpriteWorldSize(SpriteObject* spriteObject, Vec2f worldScale) const
{
	if (!spriteObject)
	{
		return Vec2f(0.f, 0.f);
	}

	PixelCoords originalSize = spriteObject->GetOriginalSize();
	float worldScaleRatio = GetPixelToWorldRatio();
	float screenRatio = GetGameScreenScale();
	return originalSize.Multiply(worldScale) * worldScaleRatio * screenRatio;
}


PixelCoords Game::GetSpriteGameScreenSize(SpriteObject* spriteObject, Vec2f scale) const
{
	if (!spriteObject)
	{
		return Vec2f(0.f, 0.f);
	}

	PixelCoords originalSize = spriteObject->GetOriginalSize();
	float screenRatio = GetGameScreenScale();
	return originalSize.Multiply(scale)  * screenRatio;
}

void Game::UpdateWidgets(float deltaTime)
{
	for (auto& widget : m_Widgets)
	{
		if (widget->IsUpdatingEnabled() && !widget->IsMarkedForDeletion())
		{
			widget->Update(deltaTime);
		}

		if (widget->IsVisible())
		{
			widget->Draw();
		}
	}
}

SpriteObject* Game::GetSprite(const std::string& path)
{
	auto iter = m_LoadedSprites.find(path);
	if (iter != m_LoadedSprites.end())
	{
		return iter->second.get();
	}

	auto& spriteEntry = m_LoadedSprites[path];
	spriteEntry = std::make_unique<SpriteObject>(path);
	return spriteEntry.get();
}

void Game::LateUpdate(float deltaTime)
{
	
}

void Game::Restart()
{
	g_Framework->Restart();
}

bool Game::Init()
{
	m_WasInitSuccessful = false;
	
	if (!SetupGameLayers())
	{
		std::cout << "Failed to setup game layers" << "\n";
		return false;
	}

	m_WasInitSuccessful = true;
	return true;
}

void Game::Update(float deltaTime)
{
	m_TimeSinceLastGarbageCollection += deltaTime;
	if (m_GarbageCollectionInterval < m_TimeSinceLastGarbageCollection)
	{
		CollectGarbage();
	}

	UpdateEntities(deltaTime);
	UpdateWidgets(deltaTime);
	UpdateCollisions(deltaTime);
	LateUpdate(deltaTime);

	if (!GetPlayer() || static_cast<Player*>(GetPlayer())->IsDead())
	{
		m_Framework->Restart();
	}
}
void Game::DrawWorld(SpriteObject* spriteObject, Vec2f worldPosition, Vec2f worldSize, bool bAutoOffset) const
{
	if (!spriteObject || !spriteObject->GetSprite())
	{
		return;
	}

	PixelCoords gameScreenPosition = WorldToScreen(worldPosition);
	PixelCoords screenPosition = m_Framework->GameToWindow(gameScreenPosition);

	PixelCoords size = worldSize / GetPixelToWorldRatio();

	m_Framework->DrawSpriteObject(spriteObject, screenPosition, size, bAutoOffset);
}

void Game::DrawScreen(SpriteObject* spriteObject, PixelCoords screenPosition, PixelCoords size, bool bAutoOffset) const
{
	if (!spriteObject || !spriteObject->GetSprite())
	{
		return;
	}

	m_Framework->DrawSpriteObject(spriteObject, m_Framework->GameToWindow(screenPosition), size, bAutoOffset);
}

void Game::OnCameraMoved(const Vec2f& delta)
{

}

PixelCoords Game::CalculateBaseWindowResolution() const
{
	return PixelCoords(100, 100);
}

void Game::SetCameraPosition(Vec2f position)
{
	Vec2f difference = position - m_CameraPosition;
	m_CameraPosition = position;
	OnCameraMoved(difference);
}


Vec2f Game::ScreenToWorld(PixelCoords screenPosition) const
{
	Vec2f worldSpaceLocation = Vec2f(screenPosition.x, GetGameScreenResolution().y - screenPosition.y) * GetPixelToWorldRatio();
	return worldSpaceLocation + GetCameraPosition();
}


PixelCoords Game::WorldToScreen(Vec2f worldPosition) const
{
	Vec2f camPos = GetCameraPosition();
	Vec2f worldSpaceLocation = worldPosition - camPos;
	float pixelRatio = GetPixelToWorldRatio();

	PixelCoords gameWindowLocation = PixelCoords(worldSpaceLocation / pixelRatio);
	PixelCoords gameScreenRes = GetGameScreenResolution();
	gameWindowLocation.y = gameScreenRes.y - gameWindowLocation.y;

	return gameWindowLocation;
}

std::unique_ptr<Framework> g_Framework;
Game* GetGame()
{
	return g_Framework->GetGame();
}
