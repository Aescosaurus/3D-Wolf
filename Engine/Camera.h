#pragma once

#include "Graphics.h"
#include "TileMap.h"
#include "Ray.h"
#include "Player.h"
#include "Surface.h"

class Camera
{
public:
	void Draw( const TileMap& tilemap,const Player& player,Graphics& gfx ) const;
	void DrawRays( Graphics& gfx ) const;
private:
	void DrawSingleRay( int col,const Ray& ray,float angle,Graphics& gfx ) const;
private:
	static constexpr float resolution = 320.0f;
	static constexpr float rayWidth = Graphics::ScreenWidth / resolution;
	static constexpr float focalLen = 0.8f;
	Surface wallSpr = "Images/Wall4.bmp";
	Surface enemySpr = "Images/Enemy.bmp";
	mutable std::vector<Ray> rays;
	Vec2 enemyPos = { 5.5f,5.0f };
};