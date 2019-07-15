#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Vec2.h"
#include "TileMap.h"

class Player
{
public:
	void Update( const Keyboard& kbd,const Mouse& mouse,float dt );
	void Draw( Graphics& gfx ) const;

	const Vec2& GetPos() const;
	float GetAngle() const;
private:
	Vec2 pos = { 10,10 };
	float angle = 0.0f;
	static constexpr float moveSpeed = 10.0f;
	static constexpr float rotSpeed = 5.0f;
};