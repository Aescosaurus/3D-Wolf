#pragma once

#include "Map.h"
#include "Keyboard.h"
#include "Graphics.h"
#include "Vec2.h"
#include "ChiliUtils.h"
#include <vector>
#include "Ray.h"

class Player
{
public:
	void Update( const Keyboard& kbd,const Map& tilemap );
	
	const Vec2& GetPos() const;
	float GetAngle() const;
	const std::vector<Ray>& GetRays() const;
private:
	static constexpr float speed = 0.07f;
	static constexpr float rotSpeed = chili::deg2rad( 5.5f );
	Vec2 pos = { float( Map::width ) / 2.0f,
		float( Map::height ) / 2.0f };
	float angle = 0.0f;
	static constexpr int fov = 90;
	static constexpr int nRays = 90;
	std::vector<Ray> rays;
};