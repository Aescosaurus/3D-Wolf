#pragma once

#include "Player.h"
#include "Graphics.h"

class Camera
{
public:
	void Draw( const Player& guy,Graphics& gfx ) const;
};