#pragma once

#include "Player.h"
#include "Graphics.h"

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
};