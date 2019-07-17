/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	MainWindow.cpp																		  *
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
#include "Resource.h"
#include "Graphics.h"
#include "ChiliException.h"
#include "Game.h"
#include <assert.h>

MainWindow::MainWindow( HINSTANCE hInst,wchar_t * pArgs )
	:
	args( pArgs ),
	hInst( hInst )
{
	// register window class
	WNDCLASSEX wc = { sizeof( WNDCLASSEX ),CS_CLASSDC,_HandleMsgSetup,0,0,
		hInst,nullptr,nullptr,nullptr,nullptr,
		wndClassName,nullptr };
	wc.hIconSm = ( HICON )LoadImage( hInst,MAKEINTRESOURCE( IDI_APPICON ),IMAGE_ICON,16,16,0 );
	wc.hIcon = ( HICON )LoadImage( hInst,MAKEINTRESOURCE( IDI_APPICON ),IMAGE_ICON,32,32,0 );
	wc.hCursor = LoadCursor( nullptr,IDC_ARROW );
	RegisterClassEx( &wc );

	// create window & get hWnd
	RECT wr;
	wr.left = 350;
	wr.right = Graphics::ScreenWidth + wr.left;
	wr.top = 100;
	wr.bottom = Graphics::ScreenHeight + wr.top;
	AdjustWindowRect( &wr,WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,FALSE );
	hWnd = CreateWindow( wndClassName,L"Chili DirectX Framework",
		// WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		WS_MAXIMIZE,
		wr.left,wr.top,wr.right - wr.left,wr.bottom - wr.top,
		nullptr,nullptr,hInst,this );

	SetWindowLong( hWnd,GWL_STYLE,0 );

	// throw exception if something went terribly wrong
	if( hWnd == nullptr )
	{
		throw Exception( _CRT_WIDE( __FILE__ ),__LINE__,
			L"Failed to get valid window handle." );
	}

	// show and update
	ShowWindow( hWnd,SW_SHOW );
	UpdateWindow( hWnd );
}

MainWindow::~MainWindow()
{
	// unregister window class
	UnregisterClass( wndClassName,hInst );
}

bool MainWindow::IsActive() const
{
	return GetActiveWindow() == hWnd;
}

bool MainWindow::IsMinimized() const
{
	return IsIconic( hWnd ) != 0;
}

void MainWindow::ShowMessageBox( const std::wstring& title,const std::wstring& message,UINT type ) const
{
	MessageBox( hWnd,message.c_str(),title.c_str(),type );
}

bool MainWindow::ProcessMessage()
{
	MSG msg;
	while( PeekMessage( &msg,nullptr,0,0,PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
		if( msg.message == WM_QUIT )
		{
			return false;
		}
		while( ShowCursor( false ) >= 0 );
	}
	return true;
}

void MainWindow::Maximize()
{
	fullscreen = true;

	SetWindowLongPtr( hWnd,SW_MAXIMIZE,0 );
	SetWindowPos( hWnd,HWND_TOP,
		0,0,GetScreenWidth(),GetScreenHeight(),
		SWP_SHOWWINDOW );
}

void MainWindow::Minimize()
{
	fullscreen = false;

	SetWindowLongPtr( hWnd,SW_SHOW,0 );
	SetWindowPos( hWnd,HWND_TOP,
		350,100,Graphics::ScreenWidth,Graphics::ScreenHeight,
		SWP_SHOWWINDOW );
}

void MainWindow::CenterMouse()
{
	RECT window;
	GetWindowRect( hWnd,&window );
	const Vei2 pos = Vei2{ int( window.left + window.right ) / 2,
		int( window.top + window.bottom ) / 2 };
	// mouse.OnMouseMove( pos.x - int( window.left ),
	// 	pos.y - int( window.top ) );
	mouse.OnMouseMove( Graphics::ScreenWidth / 2,
		Graphics::ScreenHeight / 2 );

	if( !fullscreen )
	{
		SetCursorPos( pos.x,pos.y );
	}
	else
	{
		// RECT desktop;
		// const auto hDesk = GetDesktopWindow();
		// GetWindowRect( hDesk,&desktop );
		// const Vei2 desktopPos = Vei2{ int( desktop.left + desktop.right ) / 2,
		// 	int( desktop.top + desktop.bottom ) / 2 };
		// SetCursorPos( desktopPos.x,desktopPos.y );

		SetCursorPos( Graphics::ScreenWidth / 2 - 8,
			Graphics::ScreenHeight / 2 - 8 );
	}
}

LRESULT WINAPI MainWindow::_HandleMsgSetup( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam )
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if( msg == WM_NCCREATE )
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast< CREATESTRUCTW* >( lParam );
		MainWindow* const pWnd = reinterpret_cast< MainWindow* >( pCreate->lpCreateParams );
		// sanity check
		assert( pWnd != nullptr );
		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr( hWnd,GWLP_USERDATA,reinterpret_cast< LONG_PTR >( pWnd ) );
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr( hWnd,GWLP_WNDPROC,reinterpret_cast< LONG_PTR >( &MainWindow::_HandleMsgThunk ) );
		// forward message to window class handler
		return pWnd->HandleMsg( hWnd,msg,wParam,lParam );
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc( hWnd,msg,wParam,lParam );
}

LRESULT WINAPI MainWindow::_HandleMsgThunk( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam )
{
	// retrieve ptr to window class
	MainWindow* const pWnd = reinterpret_cast< MainWindow* >( GetWindowLongPtr( hWnd,GWLP_USERDATA ) );
	// forward message to window class handler
	return pWnd->HandleMsg( hWnd,msg,wParam,lParam );
}

LRESULT MainWindow::HandleMsg( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam )
{
	switch( msg )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;

		// ************ KEYBOARD MESSAGES ************ //
	case WM_KEYDOWN:
		if( !( lParam & 0x40000000 ) || kbd.AutorepeatIsEnabled() ) // no thank you on the autorepeat
		{
			kbd.OnKeyPressed( static_cast< unsigned char >( wParam ) );
		}
		break;
	case WM_KEYUP:
		kbd.OnKeyReleased( static_cast< unsigned char >( wParam ) );
		break;
	case WM_CHAR:
		kbd.OnChar( static_cast< unsigned char >( wParam ) );
		break;
		// ************ END KEYBOARD MESSAGES ************ //

		// ************ MOUSE MESSAGES ************ //
	case WM_MOUSEMOVE:
	{
		POINTS pt = MAKEPOINTS( lParam );
		// if( pt.x > 0 && pt.x < Graphics::ScreenWidth && pt.y > 0 && pt.y < Graphics::ScreenHeight )
		{
			mouse.OnMouseMove( pt.x,pt.y );
			if( fullscreen ) KeepMouseOnScreen();
			if( !mouse.IsInWindow() )
			{
				SetCapture( hWnd );
				mouse.OnMouseEnter();
			}
		}
		if( pt.x > 0 && pt.x < Graphics::ScreenWidth && pt.y > 0 && pt.y < Graphics::ScreenHeight ) {}
		else
		{
			if( wParam & ( MK_LBUTTON | MK_RBUTTON ) )
			{
				pt.x = std::max( short( 0 ),pt.x );
				pt.x = std::min( short( Graphics::ScreenWidth - 1 ),pt.x );
				pt.y = std::max( short( 0 ),pt.y );
				pt.y = std::min( short( Graphics::ScreenHeight - 1 ),pt.y );
				mouse.OnMouseMove( pt.x,pt.y );
			}
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
				mouse.OnLeftReleased( pt.x,pt.y );
				mouse.OnRightReleased( pt.x,pt.y );
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnLeftPressed( pt.x,pt.y );
		SetForegroundWindow( hWnd );
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnRightPressed( pt.x,pt.y );
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnLeftReleased( pt.x,pt.y );
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnRightReleased( pt.x,pt.y );
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS( lParam );
		if( GET_WHEEL_DELTA_WPARAM( wParam ) > 0 )
		{
			mouse.OnWheelUp( pt.x,pt.y );
		}
		else if( GET_WHEEL_DELTA_WPARAM( wParam ) < 0 )
		{
			mouse.OnWheelDown( pt.x,pt.y );
		}
		break;
	}
	// ************ END MOUSE MESSAGES ************ //
	}

	return DefWindowProc( hWnd,msg,wParam,lParam );
}

int MainWindow::GetScreenWidth() const
{
	RECT desktop;
	const auto hDesk = GetDesktopWindow();
	GetWindowRect( hDesk,&desktop );
	return( desktop.right );
}

int MainWindow::GetScreenHeight() const
{
	RECT desktop;
	const auto hDesk = GetDesktopWindow();
	GetWindowRect( hDesk,&desktop );
	return( desktop.bottom );
}

void MainWindow::KeepMouseOnScreen()
{
	POINT realMousePos;
	GetCursorPos( &realMousePos );
	while( mouse.GetPosX() > Graphics::ScreenWidth - 5 )
	{
		--realMousePos.x;
		SetCursorPos( realMousePos.x,realMousePos.y );
		mouse.OnMouseMove( realMousePos.x,realMousePos.y );
	}
	while( mouse.GetPosY() > Graphics::ScreenHeight - 5 )
	{
		--realMousePos.y;
		SetCursorPos( realMousePos.x,realMousePos.y );
		mouse.OnMouseMove( realMousePos.x,realMousePos.y );
	}
}
