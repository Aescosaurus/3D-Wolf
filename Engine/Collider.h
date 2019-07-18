#pragma once

#include "Rect.h"
#include "TileMap.h"
#include "Vec3.h"

class Collider
{
public:
	Collider( const TileMap& map,const Rect& hitbox );
	void MoveTo( const Vec2& updatedPos );
	// z is true if there was a hit, false if no hit.
	Vec3b GetValidMove( const Vec2& startPos,const Vec2& moveTest ) const;
	const Rect& GetRect() const;
	Vec2 GetSize() const;
private:
	const TileMap* map;
	Rect hitbox;
};