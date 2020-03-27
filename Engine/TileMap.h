#pragma once

#include "Graphics.h"
#include <vector>
#include "Vec2.h"
#include "Ray.h"
#include "Vec3.h"

class TileMap
{
public:
	enum class TileType
	{
		Empty = 0,
		Wall
	};
public:
	TileMap();

	void Draw( Graphics& gfx ) const;

	Ray CastSingleRay( const Vec2& start,float angle ) const;
	TileType GetTile( const Vei2& pos ) const;
private:
	Vec3 CalcStepOffset( float rise,float run,
		float x,float y,bool inverted ) const;
public:
	static constexpr int minimapSize = 10;
private:
	Vei2 gridSize;
	std::vector<TileType> tiles;
};