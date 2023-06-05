#pragma once

//Indiviual layers for the game, are mainly used to establish basic draw order
enum class EGameLayers : unsigned char
{
	BACKGROUND,
	DEFAULT,
	ENEMIES,
	ENTITIES,
	PROJECTILES,
	BLACKHOLES,
	PLAYER,
	COUNT
};