#pragma once

#include "Vec2.h"
#include "Map.h"
#include "Graphics.h"

class Ray
{
public:
	// Please make sure dir is normalized.
	// Ray( const Vec2& start,const Vec2& dir )
	// 	:
	// 	start( start ),
	// 	pos( start ),
	// 	dir( dir )
	// {}
	Ray( const Vec2& start,float angle )
		:
		start( start ),
		pos( start ),
		dir( Vec2::FromAngle( angle ) ),
		angle( angle )
	{}

	// Returns true if touching a wall.
	bool Go( const Map& map )
	{
		pos += dir * step;
		distTraveled += step;

		auto tile = map.GetTile( pos );
		if( tile > 0 )
		{
			// TODO:
			// set int to store x to draw texture with
			// check x - round( x ) vs y - round( y ) to find the right face
			hitTileIndex = tile;
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
	float GetAngle() const
	{
		return( angle );
	}
	// Color GetColor() const
	// {
	// 	return( tileHitCol );
	// }
	int GetTileIndex() const
	{
		return( hitTileIndex );
	}
private:
	static constexpr float step = 0.1f; // From 0.9.
	Vec2 start;
	Vec2 pos;
	Vec2 dir;
	float angle;
	float distTraveled = 0.0f;
	// Color tileHitCol;
	int hitTileIndex = -1;
};