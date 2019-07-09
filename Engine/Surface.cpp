#include "Surface.h"
#include "ChiliWin.h"
#include <cassert>
#include <fstream>
#include "Graphics.h"

Surface::Surface( int width,int height ) :
	width( width ),
	height( height ),
	pixels( width * height )
{}

Surface::Surface( const std::string& filename )
{
	std::ifstream file( filename,std::ios::binary );
	assert( file );

	BITMAPFILEHEADER bmFileHeader;
	file.read( reinterpret_cast< char* >( &bmFileHeader ),
		sizeof( bmFileHeader ) );

	BITMAPINFOHEADER bmInfoHeader;
	file.read( reinterpret_cast< char* >( &bmInfoHeader ),
		sizeof( bmInfoHeader ) );

	assert( bmInfoHeader.biBitCount == 24 ||
		bmInfoHeader.biBitCount == 32 );
	assert( bmInfoHeader.biCompression == BI_RGB );

	const bool is32b = bmInfoHeader.biBitCount == 32;

	width = bmInfoHeader.biWidth;

	// Test for reverse row order and
	//  control y loop accordingly.
	int yStart;
	int yEnd;
	int dy;
	if( bmInfoHeader.biHeight < 0 )
	{
		height = -bmInfoHeader.biHeight;
		yStart = 0;
		yEnd = height;
		dy = 1;
	}
	else
	{
		height = bmInfoHeader.biHeight;
		yStart = height - 1;
		yEnd = -1;
		dy = -1;
	}

	pixels.resize( width * height );

	file.seekg( bmFileHeader.bfOffBits );
	// Padding is for 24 bit depth only.
	const int padding = ( 4 - ( width * 3 ) % 4 ) % 4;

	for( int y = yStart; y != yEnd; y += dy )
	{
		for( int x = 0; x < width; ++x )
		{
			PutPixel( x,y,Color( file.get(),file.get(),file.get() ) );
			if( is32b )
			{
				file.seekg( 1,std::ios::cur );
			}
		}
		if( !is32b )
		{
			file.seekg( padding,std::ios::cur );
		}
	}

	// *this = GetExpandedBy( Vei2{ 4,4 } );
}

Surface::Surface( const Surface& other,const RectI& clip )
{
	*this = other.GetClipped( clip );
}

Surface::Surface( const Surface& other,const Vei2& expandSize )
{
	*this = other.GetExpandedBy( expandSize );
}

Surface::Surface( const Surface& other,bool xFlipped,bool yFlipped )
{
	Surface guineaPig = { other.GetWidth(),other.GetHeight() };

	if( xFlipped ) guineaPig = other.GetXReversed();

	if( yFlipped ) guineaPig = guineaPig.GetYReversed();

	*this = guineaPig;
}

Surface::Surface( Surface&& donor )
{
	*this = std::move( donor );
}

Surface& Surface::operator=( Surface&& rhs )
{
	width = rhs.width;
	height = rhs.height;
	pixels = std::move( rhs.pixels );

	rhs.width = 0;
	rhs.height = 0;

	return( *this );
}

void Surface::PutPixel( int x,int y,Color c )
{
	assert( x >= 0 );
	assert( x < width );
	assert( y >= 0 );
	assert( y < height );
	pixels.data()[y * width + x] = c;
}

void Surface::DrawRect( int x,int y,int width,int height,Color c )
{
	for( int i = y; i < y + height; ++i )
	{
		for( int j = x; j < x + width; ++j )
		{
			PutPixel( j,i,c );
		}
	}
}

Color Surface::GetPixel( int x,int y ) const
{
	assert( x >= 0 );
	assert( x < width );
	assert( y >= 0 );
	assert( y < height );
	return pixels.data()[y * width + x];
}

int Surface::GetWidth() const
{
	return width;
}

int Surface::GetHeight() const
{
	return height;
}

Vei2 Surface::GetSize() const
{
	return Vei2{ width,height };
}

RectI Surface::GetRect() const
{
	return{ 0,width,0,height };
}

Surface Surface::GetExpandedBy( const Vei2& amount ) const
{
	Surface bigger = { amount.x * GetWidth(),amount.y * GetHeight() };

	for( int y = 0; y < this->height; ++y )
	{
		for( int x = 0; x < this->width; ++x )
		{
			bigger.DrawRect( x * amount.x,y * amount.y,
				int( amount.x ),int( amount.y ),
				GetPixel( x,y ) );
		}
	}

	return( bigger );
}

// https://rosettacode.org/wiki/Bilinear_interpolation helped a lot with this conversion code.
Surface Surface::GetInterpolatedTo( int width,int height ) const
{
	const int newWidth = width;
	const int newHeight = height;
	Surface newImage = Surface( newWidth,newHeight );
	for( int x = 0; x < newWidth; ++x )
	{
		for( int y = 0; y < newHeight; ++y )
		{
			const float gx = ( float( x ) ) / newWidth * ( GetWidth() - 1 );
			const float gy = ( float( y ) ) / newHeight * ( GetHeight() - 1 );
			const int gxi = int( gx );
			const int gyi = int( gy );
			int rgb = 0;
			const int c00 = GetPixel( gxi,gyi ).dword;
			const int c10 = GetPixel( gxi + 1,gyi ).dword;
			const int c01 = GetPixel( gxi,gyi + 1 ).dword;
			const int c11 = GetPixel( gxi + 1,gyi + 1 ).dword;
			for( int i = 0; i <= 2; ++i )
			{
				const int b00 = ( c00 >> ( i * 8 ) ) & 0xFF;
				const int b10 = ( c10 >> ( i * 8 ) ) & 0xFF;
				const int b01 = ( c01 >> ( i * 8 ) ) & 0xFF;
				const int b11 = ( c11 >> ( i * 8 ) ) & 0xFF;
				const int ble = ( int( Vec2
					::Blerp( float( b00 ),float( b10 ),
						float( b01 ),float( b11 ),
						gx - float( gxi ),gy - float( gyi ) ) ) )
					<< ( 8 * i );
				rgb = rgb | ble;
			}
			newImage.PutPixel( x,y,rgb );
		}
	}
	return newImage;
}

Surface Surface::GetNNInterpolatedTo( const Vei2& size ) const
{
	Surface temp{ size.x,size.y };
	const float xRatio = float( width ) / float( size.x );
	const float yRatio = float( height ) / float( size.y );

	for( int y = 0; y < size.y; ++y )
	{
		for( int x = 0; x < size.x; ++x )
		{
			const auto xPos = int( x * xRatio );
			const auto yPos = int( y * yRatio );
			temp.PutPixel( x,y,GetPixel( xPos,yPos ) );
		}
	}

	return( temp );
}

Surface Surface::GetXReversed() const
{
	Surface flipped = Surface{ width,height };

	for( int y = 0; y < height; ++y )
	{
		int otherX = 0;
		// Loop in reverse x.
		for( int x = width - 1; x >= 0; --x )
		{
			flipped.PutPixel( otherX,y,GetPixel( x,y ) );
			++otherX;
		}
	}

	return( flipped );
}

Surface Surface::GetYReversed() const
{
	Surface flipped = Surface{ width,height };

	int otherY = 0;
	// Loop in reverse y.
	for( int y = height - 1; y >= 0; --y )
	{
		for( int x = width - 1; x >= 0; --x )
		{
			flipped.PutPixel( x,otherY,GetPixel( x,y ) );
		}
		++otherY;
	}

	return( flipped );
}

Surface Surface::GetClipped( const RectI& clip ) const
{
	Surface clipped = { clip.GetWidth(),clip.GetHeight() };

	int i = 0;
	int j = 0;
	for( int y = clip.top; y < clip.bottom; ++y )
	{
		for( int x = clip.left; x < clip.right; ++x )
		{
			clipped.PutPixel( j,i,GetPixel( x,y ) );
			++j;
		}
		j = 0;
		++i;
	}

	return( clipped );
}