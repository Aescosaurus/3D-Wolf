#include "Player.h"

void Player::Update( const Keyboard& kbd,const Mouse& mouse,float dt )
{
	if( kbd.KeyIsPressed( 'W' ) )
	{
		pos += Vec2::FromAngle( angle ) * moveSpeed * dt;
	}
	if( kbd.KeyIsPressed( 'S' ) )
	{
		pos -= Vec2::FromAngle( angle ) * moveSpeed * dt;
	}
	if( kbd.KeyIsPressed( 'A' ) )
	{
		angle -= rotSpeed * dt;
	}
	if( kbd.KeyIsPressed( 'D' ) )
	{
		angle += rotSpeed * dt;
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
