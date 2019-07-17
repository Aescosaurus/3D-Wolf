#include "Camera.h"

void Camera::Draw( const TileMap& tilemap,const Player& player,Graphics& gfx ) const
{
	gfx.DrawRect( 0,0,
		Graphics::ScreenWidth,Graphics::ScreenHeight / 2,
		Colors::Gray );
	gfx.DrawRect( 0,Graphics::ScreenHeight / 2,
		Graphics::ScreenWidth,Graphics::ScreenHeight / 2,
		Colors::LightGray );
	
	rays.clear();
	for( int col = 0; col < int( resolution ); ++col )
	{
		const float x = float( col ) / resolution - 0.5f;
		const float angle = std::atan2( x,focalLen );
		Ray ray = tilemap.CastSingleRay( player.GetPos(),player.GetAngle() + angle );
		rays.emplace_back( ray );
		DrawSingleRay( col,ray,angle,gfx );
	}
}

void Camera::DrawRays( Graphics& gfx ) const
{
	for( const auto& ray : rays )
	{
		ray.Draw( TileMap::minimapSize,gfx );
	}
}

void Camera::DrawSingleRay( int col,const Ray& ray,float angle,Graphics& gfx ) const
{
	const float left = std::floor( float( col ) * rayWidth );
	const float z = ray.GetDist() * std::cos( angle );
	const float wallHeight = float( Graphics::ScreenHeight ) * 1.0f / z;
	const float bottom = float( Graphics::ScreenHeight ) / 2.0f *
		( 1.0f + 1.0f / z );

	// gfx.DrawRectSafe( int( left ),int( bottom - wallHeight ),
	// 	int( std::ceil( rayWidth ) ),int( wallHeight ),Colors::Red );

	const int texX = int( ray.offset * float( wallSpr.GetWidth() ) );

	gfx.DrawSprite( RectI{ texX,texX + 1,0,wallSpr.GetHeight() },
		RectI{ int( left ),int( left + std::ceil( rayWidth ) ),
		int( bottom - wallHeight ),int( bottom ) },
		wallSpr );
}