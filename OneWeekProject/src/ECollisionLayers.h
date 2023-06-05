#pragma once

typedef unsigned int CollisionMask;
enum class ECollisionLayers : CollisionMask
{
	ENTITY,		//0
	PLAYER,		//1
	ENEMY,		//2
	PLATFORM,	//3
	PROJECTILE,	//4
	ABILITIES,	//5
	COUNT
};
