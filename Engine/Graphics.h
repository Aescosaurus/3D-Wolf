/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
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
#pragma once
#include "ChiliWin.h"
#include <d3d11.h>
#include <wrl.h>
#include "ChiliException.h"
#include "Colors.h"
#include "Vec2.h"
#include "Surface.h"

class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
private:
	// vertex format for the framebuffer fullscreen textured quad
	struct FSQVertex
	{
		float x,y,z;		// position
		float u,v;			// texcoords
	};
public:
	Graphics( class HWNDKey& key );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	void EndFrame();
	void BeginFrame();
	void PutPixel( int x,int y,int r,int g,int b )
	{
		PutPixel( x,y,{ unsigned char( r ),unsigned char( g ),unsigned char( b ) } );
	}
	void PutPixel( int x,int y,Color c );
	void DrawRect( int x,int y,int width,int height,Color c );
	void DrawRectSafe( int x,int y,int width,int height,Color c );
	void DrawLine( Vec2 p0,Vec2 p1,Color c );
	void DrawSprite( RectI src,RectI dest,const Surface& sprite )
	{
		if( src.left < 0 ) src.left = 0;
		if( src.right > sprite.GetWidth() - 1 ) src.right = sprite.GetWidth() - 1;
		if( src.top < 0 ) src.top = 0;
		if( src.bottom > sprite.GetHeight() - 1 ) src.bottom = sprite.GetHeight() - 1;

		if( dest.left < 0 ) dest.left = 0;
		if( dest.right > ScreenWidth - 1 ) dest.right = ScreenWidth - 1;
		if( dest.top < 0 ) dest.top = 0;
		if( dest.bottom > ScreenHeight - 1 ) dest.bottom = ScreenHeight - 1;

		for( int y = dest.top; y < dest.bottom; ++y )
		{
			for( int x = dest.left; x < dest.right; ++x )
			{
				// const auto pixX = src.left + ( float( x - dest.left ) /
				// 	float( src.GetWidth() ) );
				// const auto pixY = src.top + int( float( y - dest.top ) /
				// 	float( src.GetHeight() ) );
				const auto xPercent = float( x - dest.left ) /
					float( dest.GetWidth() );
				const auto yPercent = float( y - dest.top ) /
					float( dest.GetHeight() );

				const auto pixX = src.left + int( xPercent *
					float( src.GetWidth() ) );
				const auto pixY = src.top + int( yPercent *
					float( src.GetHeight() ) );

				PutPixel( x,y,sprite.GetPixel( pixX,pixY ) );
			}
		}
	}
	~Graphics();
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	Color*                                              pSysBuffer = nullptr;
public:
	static constexpr int ScreenWidth = 800;
	static constexpr int ScreenHeight = 600;
};