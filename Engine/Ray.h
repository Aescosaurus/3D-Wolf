#pragma once

#include "Vec2.h"

class Ray
{
public:
	Ray( const Vec2& start,float angle )
		:
		start( start ),
		pos( start ),
		dir( Vec2::FromAngle( angle ) )
	{}

	void Draw( float expand,Graphics& gfx ) const
	{
		gfx.DrawLine( start * expand,pos * expand,Colors::Green );
	}

	float GetDist() const
	{
		return( ( pos - start ).GetLength() );
	}
	float GetDistSq() const
	{
		return( ( pos - start ).GetLengthSq() );
	}
public:
	Vec2 start;
	Vec2 pos;
	Vec2 dir;
	int height = -1;
	float offset = -1.0f;
	int col = -1;
	float angle = -1.0f;
	static constexpr float step = 0.1f;
};