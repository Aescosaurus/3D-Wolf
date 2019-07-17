#include "Player.h"
#include "ChiliUtils.h"

void Player::Update( const Keyboard& kbd,Mouse& mouse,
	MainWindow& wnd,const TileMap& map,float dt )
{
	angle += ( mouse.GetPosX() - lastMousePos.x ) * rotSpeed * dt;

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
	if( kbd.KeyIsPressed( 'A' ) )
	{
		dx -= std::cos( angle + chili::pi / 2.0f ) * moveSpeed * dt;
		dy -= std::sin( angle + chili::pi / 2.0f ) * moveSpeed * dt;
	}
	if( kbd.KeyIsPressed( 'D' ) )
	{
		dx += std::cos( angle + chili::pi / 2.0f ) * moveSpeed * dt;
		dy += std::sin( angle + chili::pi / 2.0f ) * moveSpeed * dt;
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

	wnd.CenterMouse();
	lastMousePos = mouse.GetPos();
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
