#pragma once

#include "Vec2.h"
#include "Map.h"
#include "Graphics.h"

class Ray
{
public:
	// Please make sure dir is normalized.
	Ray( const Vec2& start,const Vec2& dir )
		:
		start( start ),
		pos( start ),
		dir( dir )
	{}

	// Returns true if touching a wall.
	bool Go( const Map& map )
	{
		pos += dir * step;
		distTraveled += step;

		auto tile = map.GetTile( pos );
		if( tile >= 1 )
		{
			if( tile == 1 ) tileHitCol = Colors::Gray;
			else if( tile == 2 ) tileHitCol = Colors::Cyan;
			else if( tile == 3 ) tileHitCol = Colors::Blue;
			return( true );
		}
		else return( false );
	}
	void Draw( int xMult,int yMult,Graphics& gfx ) const
	{
		gfx.DrawLine( Vec2{ start.x * float( xMult ),
			start.y * float( yMult ) },
			Vec2{ pos.x * float( xMult ),
			pos.y * float( yMult ) },Colors::Green );
	}
	float GetDist() const
	{
		return( distTraveled );
	}
	Color GetColor() const
	{
		return( tileHitCol );
	}
private:
	static constexpr float step = 0.1f; // From 0.9.
	Vec2 start;
	Vec2 pos;
	Vec2 dir;
	float distTraveled = 0.0f;
	Color tileHitCol;
};