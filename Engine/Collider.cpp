#include "Collider.h"

Collider::Collider( const TileMap& map,const Rect& hitbox )
	:
	map( &map ),
	hitbox( hitbox )
{}

void Collider::MoveTo( const Vec2& updatedPos )
{
	hitbox.MoveTo( updatedPos - hitbox.GetSize() / 2.0f );
}

Vec3b Collider::GetValidMove( const Vec2& startPos,const Vec2& moveTest ) const
{
	// We gotta test every point on the rect.
	std::vector<Vec2> positions;
	positions.emplace_back( Vec2{ hitbox.left,hitbox.top } );
	positions.emplace_back( Vec2{ hitbox.right,hitbox.top } );
	positions.emplace_back( Vec2{ hitbox.left,hitbox.bottom } );
	positions.emplace_back( Vec2{ hitbox.right,hitbox.bottom } );
	// These next ones make it way more accurate but computationally slower.
	const auto center = hitbox.GetCenter();
	positions.emplace_back( center );
	positions.emplace_back( Vec2{ hitbox.left,center.y } );
	positions.emplace_back( Vec2{ center.x,hitbox.top } );
	positions.emplace_back( Vec2{ hitbox.right,center.y } );
	positions.emplace_back( Vec2{ center.x,hitbox.bottom } );

	// Start true, if any point cannot move then none can.
	Vec2_<bool> isGood = { true,true };

	for( const auto& pos : positions )
	{
		// If tile at one corner is not empty, don't go that way.
		if( map->GetTile( Vei2( pos + Vec2{ moveTest.x,0.0f } ) ) !=
			TileMap::TileType::Empty )
		{
			isGood.x = false;
		}

		if( map->GetTile( Vei2( pos + Vec2{ 0.0f,moveTest.y } ) ) !=
			TileMap::TileType::Empty )
		{
			isGood.y = false;
		}
	}

	// z tells whether you hit or not.
	auto goodPos = Vec3b{ 0.0f,0.0f,!isGood.x || !isGood.y };

	if( isGood.x ) goodPos.x += moveTest.x;
	if( isGood.y ) goodPos.y += moveTest.y;

	return( goodPos );
}

const Rect& Collider::GetRect() const
{
	return( hitbox );
}

Vec2 Collider::GetSize() const
{
	return( Vec2{ float( hitbox.GetWidth() ),
		float( hitbox.GetHeight() ) } );
}