#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Vec2.h"
#include "TileMap.h"
#include "MainWindow.h"
#include "Collider.h"

class Player
{
public:
	Player( const TileMap& map );

	void Update( const Keyboard& kbd,Mouse& mouse,
		MainWindow& wnd,const TileMap& map,float dt );
	void Draw( Graphics& gfx ) const;

	const Vec2& GetPos() const;
	float GetAngle() const;
private:
	static constexpr Vec2 size = { 0.4,0.4 };
	static constexpr float moveSpeed = 5.0f;
	static constexpr float rotSpeed = 2.0f / 25.0f;
	Vec2 pos = { 10,10 };
	float angle = 0.0f;
	Collider coll;
	Vei2 lastMousePos = { 0,0 };
};