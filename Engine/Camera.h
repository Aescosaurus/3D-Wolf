#pragma once

#include "Player.h"
#include "Graphics.h"
#include "Surface.h"

class Camera
{
public:
	void Draw( const Player& guy,Graphics& gfx ) const;
private:
	static constexpr Color colors[] =
	{
		Colors::Gray,
		Colors::Cyan,
		Colors::Green
	};
	const Surface wallSpr = "Images/Wall.bmp";
	const Surface wallSpr2 = "Images/Wall3.bmp";
};