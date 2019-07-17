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

	void SetOffset( float offset )
	{
		texOffset = offset;
	}
	void SetPos( const Vec2& pos )
	{
		this->pos = pos;
	}
	void SetHeight( int height )
	{
		this->height = height;
	}

	float GetOffset() const
	{
		return( texOffset );
	}
	const Vec2& GetPos() const
	{
		return( pos );
	}
	float GetDist() const
	{
		return( ( pos - start ).GetLength() );
	}
	int GetHeight() const
	{
		return( height );
	}
private:
	Vec2 start;
	Vec2 pos;
	Vec2 dir;
	int height;
	float texOffset = -1.0f;
	static constexpr float step = 0.1f;
};