#pragma once

#include "Player.h"
#include "Graphics.h"
#include "Surface.h"

class Camera
{
public:
	void Draw( const Player& guy,Graphics& gfx ) const;
private:
	const Surface sprites[3] =
	{
		"Images/Wall4.bmp",
		"Images/Wall4.bmp",
		"Images/Wall4.bmp"
	};
	static constexpr Vei2 wallSize = { 32,32 };
	static constexpr float focalLen = 0.6f; // from 0.8
};