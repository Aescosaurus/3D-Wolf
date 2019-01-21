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
	float rayX = 0.0f;
	const int screenCenter = Graphics::ScreenHeight / 2;
	for( const auto& ray : rays )
	{
		// Ray height depends on distance.
		const int rayHeight = Graphics::ScreenHeight -
			int( ray.GetDist() * 45.0f );

		gfx.DrawRect( int( round( rayX ) ),
			screenCenter - rayHeight / 2,
			int( round( rayWidth ) ),
			rayHeight,ray.GetColor() );

		rayX -= rayWidth;
	}
	tilemap.Draw( guy,gfx );
}
