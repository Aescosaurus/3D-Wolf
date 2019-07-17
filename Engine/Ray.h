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

	void Advance()
	{
		pos += dir * step;
	}
	void SetOffset( float offset )
	{
		texOffset = offset;
	}

	float GetOffset() const
	{
		return( texOffset );
	}
	const Vec2& GetPos() const
	{
		return( pos );
	}
	Vec2 GetPrevPos() const
	{
		return( pos - dir * step );
	}
	float GetDist() const
	{
		return( ( pos - start ).GetLength() );
	}
private:
	Vec2 start;
	Vec2 pos;
	Vec2 dir;
	float texOffset = -1.0f;
	static constexpr float step = 0.1f;
};