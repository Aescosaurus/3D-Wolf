#include "TileMap.h"
#include <fstream>
#include <cassert>

TileMap::TileMap()
{
	std::ifstream in{ "Levels/Level1.txt" };
	assert( in.good() );

	gridSize = { 0,0 };
	bool countedWidth = false;

	for( char c = in.get(); in.good(); c = in.get() )
	{
		switch( c )
		{
		case '\n':
			if( !countedWidth ) countedWidth = true;
			++gridSize.y;
			break;
		default:
			if( !countedWidth ) ++gridSize.x;
			tiles.emplace_back( TileType( c - '0' ) );
			break;
		}
	}

	++gridSize.y;
}

void TileMap::Draw( Graphics& gfx ) const
{
	for( int y = 0; y < gridSize.y; ++y )
	{
		for( int x = 0; x < gridSize.x; ++x )
		{
			auto col = Colors::White;
			const auto tile = GetTile( Vei2{ x,y } );
			switch( tile )
			{
			case TileType::Wall:
				col = Colors::Gray;
				break;
			}
			gfx.DrawRect( x * minimapSize,y * minimapSize,
				minimapSize,minimapSize,col );
		}
	}
}

Ray TileMap::CastSingleRay( const Vec2& start,float angle ) const
{
	Ray temp{ start,angle };
	bool foundWall = false;
	const auto sin = std::sin( angle );
	const auto cos = std::cos( angle );
	
	// while( GetTile( temp.GetPos() ) == TileType::Empty )
	while( !foundWall )
	{
		const Vec3 stepX = CalcStepOffset( sin,
			cos,temp.GetPos().x,temp.GetPos().y,false );
		const Vec3 stepY = CalcStepOffset( cos,
			sin,temp.GetPos().y,temp.GetPos().x,true );

		if( stepX.z < stepY.z )
		{
			temp.SetPos( stepX );
			temp.SetOffset( stepX.y - std::floor( stepX.y ) );

			const auto dx = cos < 0 ? 1 : 0;
			if( GetTile( Vei2( temp.GetPos() ) -
				Vei2{ dx,0 } ) != TileType::Empty )
			{
				foundWall = true;
			}
		}
		else
		{
			temp.SetPos( stepY );
			temp.SetOffset( stepY.x - std::floor( stepY.x ) );

			const auto dy = sin < 0 ? 1 : 0;
			if( GetTile( Vei2( temp.GetPos() ) -
				Vei2{ 0,dy } ) != TileType::Empty )
			{
				foundWall = true;
			}
		}
	}

	return( temp );
}

TileMap::TileType TileMap::GetTile( const Vei2& pos ) const
{
	assert( pos.x >= 0 );
	assert( pos.x < gridSize.x );
	assert( pos.y >= 0 );
	assert( pos.y < gridSize.y );

	return( tiles[pos.y * gridSize.x + pos.x] );
}

Vec3 TileMap::CalcStepOffset( float rise,float run,
	float x,float y,bool inverted ) const
{
	if( run == 0.0f ) return( Vec3{ 0.0f,0.0f,-1.0f } );

	const float dx = ( ( run > 0 )
		? std::floor( x + 1.0f ) - x
		: std::ceil( x - 1.0f ) - x );
	const float dy = dx * ( rise / run );

	return( Vec3{ inverted ? y + dy : x + dx,
		inverted ? x + dx : y + dy,
		dx * dx + dy * dy } );
}
