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

	while( GetTile( Vei2( temp.GetPos() ) ) == TileType::Empty )
	{
		temp.Advance();
	}

	bool isXOrY; // true = x false = y
	const auto& rayPos = temp.GetPos();
	const auto oldRayPos = temp.GetPrevPos();

	isXOrY = int( rayPos.x ) == int( oldRayPos.x );

	const Vec3 stepX = CalcStepOffset( std::sin( angle ),
		std::cos( angle ),temp.GetPos().x,temp.GetPos().y,false );
	const Vec3 stepY = CalcStepOffset( std::cos( angle ),
		std::sin( angle ),temp.GetPos().y,temp.GetPos().x,true );

	// if( stepX.z < stepY.z ) temp.SetOffset( stepX.y - std::floor( stepX.y ) );
	// else temp.SetOffset( stepY.x - std::floor( stepY.x ) );

	if( isXOrY ) temp.SetOffset( stepX.y - std::floor( stepX.y ) );
	else temp.SetOffset( stepY.x - std::floor( stepY.x ) );

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
