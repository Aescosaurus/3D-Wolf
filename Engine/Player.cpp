#include "Player.h"

void Player::Update( const Keyboard& kbd,const Map& tilemap )
{
	if( kbd.KeyIsPressed( 'W' ) ) pos += Vec2::FromAngle( angle ) * speed;
	if( kbd.KeyIsPressed( 'S' ) ) pos -= Vec2::FromAngle( angle ) * speed;
	if( kbd.KeyIsPressed( 'A' ) ) angle -= rotSpeed;
	if( kbd.KeyIsPressed( 'D' ) ) angle += rotSpeed;

	// TODO: Collision detection of some kind.

	rays.clear();
	for( int i = -fov / 2; i < fov / 2; i += fov / nRays )
	{
		rays.emplace_back( Ray{ pos,Vec2::FromAngle(
			angle - chili::deg2rad( float( i ) ) ) } );

		while( !rays.back().Go( tilemap ) );
	}
}

const Vec2& Player::GetPos() const
{
	return( pos );
}

float Player::GetAngle() const
{
	return( angle );
}

const std::vector<Ray>& Player::GetRays() const
{
	return( rays );
}