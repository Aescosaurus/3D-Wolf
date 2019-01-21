#include "Map.h"
#include "Player.h"

void Map::Draw( const Player& guy,Graphics& gfx ) const
{
	const int tileWidth = 10;
	const int tileHeight = 10;
	for( int y = 0; y < height; ++y )
	{
		for( int x = 0; x < width; ++x )
		{
			auto col = Colors::White;
			if( tileMap[y * width + x] == 1 ) col = Colors::LightGray;

			gfx.DrawRect( x * tileWidth,y * tileHeight,
				tileWidth,tileHeight,col );
		}
	}

	for( const auto& ray : guy.GetRays() )
	{
		ray.Draw( tileWidth,tileHeight,gfx );
	}

	gfx.DrawCircle( int( guy.GetPos().x * float( tileWidth ) ),
		int( guy.GetPos().y * float( tileHeight ) ),
		5,Colors::Red );
	gfx.DrawLine( Vec2{ guy.GetPos().x * float( tileWidth ),
		guy.GetPos().y * float( tileHeight ) },
		Vec2{ ( guy.GetPos().x + cos( guy.GetAngle() ) ) * tileWidth,
		( guy.GetPos().y + sin( guy.GetAngle() ) ) * tileHeight } * 1.0f,
		Colors::Red );
}

int Map::GetTile( const Vec2& mapPos ) const
{
	return( tileMap[int( mapPos.y ) * width +
		int( mapPos.x )] );
}
