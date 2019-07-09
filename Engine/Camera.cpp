#include "Camera.h"
#include "SpriteEffect.h"

void Camera::Draw( const Player& guy,Graphics& gfx ) const
{
	gfx.DrawRect( 0,0,Graphics::ScreenWidth,
		Graphics::ScreenHeight / 2,Colors::DarkGray );
	gfx.DrawRect( 0,Graphics::ScreenHeight / 2,
		Graphics::ScreenWidth,Graphics::ScreenHeight / 2,
		Colors::LightGray );

	const auto& rays = guy.GetRays();
	// const float resolution = float( rays.size() );
	const int nRays = int( rays.size() );
	const float resolution = 320.0f;
	const float rayWidth = float( Graphics::ScreenWidth ) / resolution;
	float rayX = float( Graphics::ScreenWidth ) - rayWidth;
	for( int col = 0; col < int( resolution ); ++col )
	{
		const auto& ray = rays[int( float( col ) /
			( resolution / float( nRays ) ) )];

		// Famcy maffs.
		const auto x = col / resolution - 0.5f;
		const auto angle = std::atan2( x,focalLen );
		const auto z = ray.GetDist() * std::cos( angle );
		const auto rayHeight = Graphics::ScreenHeight * ( 1.0f / z );
		const auto rayBot = float( Graphics::ScreenHeight ) / 2.0f *
			( 1.0f + 1.0f / z );

		// gfx.DrawRectDim( std::max( 0,int( std::round( rayX ) ) ),
		// 	std::max( 0,int( rayBot - rayHeight ) ),
		// 	std::min( Graphics::ScreenWidth - 1,
		// 		int( std::round( rayX ) ) + int( std::round( rayWidth ) ) ),
		// 	std::min( Graphics::ScreenHeight - 1,int( rayBot ) ),
		// 	colors[ray.GetTileIndex() - 1] );

		gfx.DrawSpriteDim( std::max( 0,int( std::round( rayX ) ) ),
			std::max( 0,int( rayBot - rayHeight ) ),
			std::min( Graphics::ScreenWidth - 1,
				int( std::round( rayX ) ) + int( std::round( rayWidth ) ) ),
			std::min( Graphics::ScreenHeight - 1,int( rayBot ) ),
			int( ray.GetTexX() * float( wallSize.x ) ),
			// std::max( 1,int( ray.GetTexWidth() ) ),
			int( z ),
			sprites[ray.GetTileIndex() - 1],SpriteEffect::Copy{} );

		rayX -= rayWidth;
	}
}
