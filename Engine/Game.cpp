/******************************************************************************************
 *	Chili DirectX Framework Version 16.07.20											  *
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	guy.Update( wnd.kbd,tilemap );
}

void Game::ComposeFrame()
{
	gfx.DrawRect( 0,0,Graphics::ScreenWidth,
		Graphics::ScreenHeight / 2,Colors::DarkGray );
	gfx.DrawRect( 0,Graphics::ScreenHeight / 2,
		Graphics::ScreenWidth,Graphics::ScreenHeight / 2,
		Colors::LightGray );

	const auto& rays = guy.GetRays();
	const float rayWidth = float( Graphics::ScreenWidth ) /
		float( rays.size() );
	float rayX = float( Graphics::ScreenWidth ) - rayWidth;
	const int screenCenter = Graphics::ScreenHeight / 2;
	const float resolution = float( rays.size() );
	// for( const auto& ray : rays )
	for( int col = 0; col < int( resolution ); ++col )
	{
		const auto& ray = rays[col];

		// Ray height depends on distance.
		// const int rayHeight = Graphics::ScreenHeight -
		// 	int( ray.GetDist() * 45.0f );
		// 
		// gfx.DrawRect( int( round( rayX ) ),
		// 	screenCenter - rayHeight / 2,
		// 	int( round( rayWidth ) ),
		// 	rayHeight,ray.GetColor() );

		// Famcy maffs.
		const auto x = col / resolution - 0.5f;
		const auto angle = std::atan2( x,0.8f );
		const auto z = ray.GetDist() * std::cos( angle );
		const auto rayHeight = Graphics::ScreenHeight * ( 1.0f / z );
		const auto rayBot = float( Graphics::ScreenHeight ) / 2.0f *
			( 1.0f + 1.0f / z );

		gfx.DrawRectDim( int( std::round( rayX ) ),
			int( rayBot - rayHeight ),
			int( std::round( rayX ) ) + int( std::round( rayWidth ) ),
			int( rayBot ),ray.GetColor() );

		// gfx.DrawLine( Vec2{ float( Graphics::ScreenWidth / 2 ),
		// 	float( Graphics::ScreenHeight - 1 ) },
		// 	Vec2{ rayX,float( screenCenter - rayHeight / 2 + rayHeight ) },
		// 	Colors::Green );

		rayX -= rayWidth;
	}
	tilemap.Draw( guy,gfx );
}
