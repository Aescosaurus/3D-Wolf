#include "Player.h"

void Player::Update( const Keyboard& kbd,const Mouse& mouse,
	const TileMap& map,float dt )
{
	if( kbd.KeyIsPressed( 'A' ) )
	{
		angle -= rotSpeed * dt;
	}
	if( kbd.KeyIsPressed( 'D' ) )
	{
		angle += rotSpeed * dt;
	}

	float dx = 0.0f;
	float dy = 0.0f;

	if( kbd.KeyIsPressed( 'W' ) )
	{
		dx += std::cos( angle ) * moveSpeed * dt;
		dy += std::sin( angle ) * moveSpeed * dt;
	}
	if( kbd.KeyIsPressed( 'S' ) )
	{
		dx -= std::cos( angle ) * moveSpeed * dt;
		dy -= std::sin( angle ) * moveSpeed * dt;
	}

	if( map.GetTile( Vei2( Vec2{ pos.x + dx,pos.y } ) ) ==
		TileMap::TileType::Empty )
	{
		pos.x += dx;
	}
	if( map.GetTile( Vei2( Vec2{ pos.x,pos.y + dy } ) ) ==
		TileMap::TileType::Empty )
	{
		pos.y += dy;
	}
}

void Player::Draw( Graphics& gfx ) const
{
	const auto drawPos = Vei2( pos * float( TileMap::minimapSize ) );

	gfx.DrawRect( drawPos.x - 2,drawPos.y - 2,5,5,Colors::Red );

	gfx.DrawLine( drawPos,
		drawPos + Vec2::FromAngle( angle ) * 10.0f,
		Colors::Red );
}

const Vec2& Player::GetPos() const
{
	return( pos );
}

float Player::GetAngle() const
{
	return( angle );
}
