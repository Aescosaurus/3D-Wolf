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

		const auto tile = map.GetTile( pos );
		if( tile > 0 )
		{
			// const auto stepX = CalcTextureOffset( std::sin( angle ),
			// 	std::cos( angle ),pos.x,pos.y,false );
			// const auto stepY = CalcTextureOffset( std::cos( angle ),
			// 	std::sin( angle ),pos.y,pos.x,true );
			// 
			// // textureX = stepX.GetLengthSq() < stepY.GetLengthSq()
			// // 	? stepX.y : stepY.x;
			// if( stepX.GetLengthSq() < stepY.GetLengthSq() )
			// {
			// 	textureX = stepX.y;
			// 	hitColor = Colors::Red;
			// }
			// else
			// {
			// 	textureX = stepY.x;
			// 	hitColor = Colors::Cyan;
			// }
			// 
			// textureX = textureX - std::floor( textureX );

			// const auto xDiff = pos.x - std::round( pos.x );
			// const auto yDiff = pos.y - std::round( pos.y );
			// if( xDiff > yDiff )
			// {
			// 	textureX = xDiff;
			// 	hitColor = Colors::Red;
			// }
			// else
			// {
			// 	textureX = yDiff;
			// 	hitColor = Colors::Blue;
			// }

			const auto xDiff = std::abs( pos.x - std::floor( pos.x ) );
			const auto yDiff = std::abs( pos.y - std::floor( pos.y ) );
			if( xDiff < yDiff )
			{
				textureX = yDiff;
				hitColor = Colors::Red;
			}
			else
			{
				textureX = xDiff;
				hitColor = Colors::Blue;
			}

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

		gfx.DrawCircle( int( pos.x * xMult ),
			int( pos.y * yMult ),
			2,hitColor );
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
	float GetTexX() const
	{
		return( textureX );
	}
	float GetTexWidth() const
	{
		return( distTraveled );
	}
private:
	// TODO: Make this better.
	Vec2 CalcTextureOffset( float rise,float run,float x,float y,bool inverted ) const
	{
		if( run == 0.0f )
		{
			return( Vec2{ 0.0f,0.0f } );
		}

		const auto dx = run > 0.0f
			? std::floor( x + 1.0f ) - x
			: std::ceil( x - 1.0f ) - x;
		const auto dy = dx * ( rise / run );

		return( Vec2{ inverted ? y + dy : x + dx,
			inverted ? x + dx : y + dy } );
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
	float textureX = -1.0f;
	Color hitColor;
};