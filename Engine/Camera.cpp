#include "Camera.h"
#include "ChiliUtils.h"

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
		Ray ray = tilemap.CastSingleRay( player.GetPos(),
			player.GetAngle() + angle );
		// std::vector<Ray> objects = tilemap.WalkRay( ray );
		// for( auto& obj : objects )
		// {
		// 	obj.col = col;
		// 	obj.angle = angle;
		// 	objects.emplace_back( obj );
		// }
		ray.col = col;
		ray.angle = angle;
		rays.emplace_back( ray );
	}

	std::sort( rays.begin(),rays.end(),[]
	( const Ray& r1,const Ray& r2 )
	{
		return( r1.GetDistSq() > r2.GetDistSq() );
	} );

	for( const auto& ray : rays )
	{
		DrawSingleRay( ray.col,ray,ray.angle,gfx );
	}
	for( const auto& obj : objects )
	{
		DrawSingleRay( obj.col,obj,obj.angle,gfx );
	}

	// const auto viewDist = float( Graphics::ScreenWidth / 2 ) / std::tan( 45.0f );
	// const auto enemyPos = Vec2{ 5.5f,5.0f };
	// const auto posDiff = enemyPos - player.GetPos();
	// const auto angle = std::atan2( posDiff.y,posDiff.x ) - player.GetAngle();
	// const auto distSq = posDiff.x * posDiff.x + posDiff.y * posDiff.y;
	// const auto dist = std::sqrt( distSq );
	// const auto size = viewDist / ( std::cos( angle ) * dist );
	// const auto x = std::tan( angle ) * viewDist;

	const auto diff = player.GetPos() - enemyPos;
	const auto angle = abs( std::atan2( diff.y,diff.x ) );
	Ray ray{ player.GetPos(),angle };
	ray.pos = enemyPos;
	rays.emplace_back( ray );

	const float minX = 0.0f / resolution - 0.5f;
	const float maxX = resolution / resolution - 0.5f;
	const float minAngle = std::atan2( minX,focalLen ) - player.GetAngle();
	const float maxAngle = std::atan2( maxX,focalLen ) - player.GetAngle();

	// if( angle > minAngle && angle < maxAngle )
	// if( abs( player.GetAngle() - angle ) < resolution / 2.0f )
	{
		const auto x = float( Graphics::ScreenWidth ) -
			( player.GetAngle() - chili::pi ) /
			( maxAngle - minAngle ) *
			float( Graphics::ScreenWidth );
		const float z = ray.GetDist() * std::cos( angle );
		const float wallHeight = float( Graphics::ScreenHeight ) * 1.0f / z;
		const float bottom = float( Graphics::ScreenHeight ) / 2.0f *
			( 1.0f + 1.0f / z );
		
		gfx.DrawRectSafe( int( x ),int( bottom - wallHeight ),
			50,int( wallHeight ),Colors::Red );
		// gfx.DrawRectSafe( int( x ),150,150,50,Colors::Red );
	}

	// const float left = std::floor( float( col ) * rayWidth );
	// const float z = ray.GetDist() * std::cos( angle );
	// const float wallHeight = float( Graphics::ScreenHeight ) * 1.0f / z;
	// const float bottom = float( Graphics::ScreenHeight ) / 2.0f *
	// 	( 1.0f + 1.0f / z );
	// 
	// gfx.DrawRectSafe( int( left ),int( bottom - wallHeight ),
	// 	50,int( wallHeight ),Colors::Red );
}

void Camera::DrawRays( Graphics& gfx ) const
{
	for( const auto& ray : rays )
	{
		ray.Draw( TileMap::minimapSize,gfx );
	}

	// gfx.DrawRect( int( enemyPos.x ) * TileMap::minimapSize,
	// 	int( enemyPos.y ) * TileMap::minimapSize,
	// 	TileMap::minimapSize,TileMap::minimapSize,
	// 	Colors::Red );
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

void Camera::DrawSingleObj( int col,const Ray& ray,float angle,Graphics& gfx ) const
{
	const float left = std::floor( float( col ) * rayWidth );
	const float z = ray.GetDist() * std::cos( angle );
	const float wallHeight = float( Graphics::ScreenHeight ) * 1.0f / z;
	const float bottom = float( Graphics::ScreenHeight ) / 2.0f *
		( 1.0f + 1.0f / z );

	gfx.DrawRectSafe( int( left ),int( bottom - wallHeight ),
		int( std::ceil( rayWidth ) ),int( wallHeight ),Colors::Red );

	// const int texX = int( ray.offset * float( wallSpr.GetWidth() ) );

	// gfx.DrawSprite( RectI{ texX,texX + 1,0,wallSpr.GetHeight() },
	// 	RectI{ int( left ),int( left + std::ceil( rayWidth ) ),
	// 	int( bottom - wallHeight ),int( bottom ) },
	// 	wallSpr );
}
