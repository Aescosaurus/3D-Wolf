#pragma once

#include <cmath>
#include "Vec2.h"

class Matrix
{
public:
	// static Matrix Rotation( float angle )
	// {
	// 	return( Matrix{ cos( angle ),-sin( angle ),
	// 		sin( angle ),cos( angle ) } );
	// }
	static Matrix Scaling( float xStretch,float yStretch )
	{
		return( Matrix{ xStretch,0,
			yStretch,0 } );
	}
	Vec2 operator*( const Vec2& rhs ) const
	{
		Vec2 start = { 0.0f,0.0f };
		start.x = data[0][0] * rhs.x + data[0][1] * rhs.y;
		start.y = data[1][0] * rhs.x + data[1][1] * rhs.y;
		return( start );
	}
private:
	Matrix( float a,float b,float c,float d )
	{
		data[0][0] = a;
		data[0][1] = b;
		data[1][0] = c;
		data[1][1] = d;
	}
private:
	static constexpr int size = 2;
	float data[size][size];
};