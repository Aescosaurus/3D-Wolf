#pragma once

#include "Graphics.h"
#include "Vec2.h"

class Map
{
public:
	void Draw( const class Player& guy,Graphics& gfx ) const;

	int GetTile( const Vec2& mapPos ) const;
public:
	static constexpr int width = 20;
	static constexpr int height = 15;
private:
	static constexpr int tileWidth = Graphics::ScreenWidth / width;
	static constexpr int tileHeight = Graphics::ScreenHeight / height;
	static constexpr int tileMap[width * height] =
	{
		1,1,2,1,2,3,1,2,1,3,2,2,1,3,2,3,1,3,2,1,
		3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,
		2,0,0,0,1,3,1,2,0,0,0,0,0,0,0,0,0,0,0,2,
		1,0,0,0,0,0,0,3,1,0,0,0,0,0,0,0,0,0,0,1,
		3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,
		2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
		2,0,0,0,0,0,0,0,0,0,0,2,0,1,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,3,
		3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
		3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,2,1,3,1,2,3,3,1,2,1,3,1,2,3,1,3,3,2,1
	};
};