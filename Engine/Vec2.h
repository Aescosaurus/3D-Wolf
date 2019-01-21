#pragma once

#include <cmath>

template<typename T>
class Vec2_
{
public:
	constexpr Vec2_() = default;
	constexpr Vec2_( T x,T y )
		:
		x( x ),
		y( y )
	{}
	constexpr Vec2_( T amount )
		:
		x( amount ),
		y( amount )
	{
	}
	// template<typename U>
	// constexpr Vec2_( const Vec2_<U>& src )
	// 	:
	// 	x( T( src.x ) ),
	// 	y( T( src.y ) )
	// {}

	template<typename U>
	constexpr operator Vec2_<U>() const
	{
		return Vec2_<U>( U( x ),U( y ) );
	}

	constexpr Vec2_ operator+( const Vec2_& rhs ) const
	{
		return Vec2_( x + rhs.x,y + rhs.y );
	}
	constexpr Vec2_& operator+=( const Vec2_& rhs )
	{
		return *this = *this + rhs;
	}
	constexpr Vec2_ operator*( T rhs ) const
	{
		return Vec2_( x * rhs,y * rhs );
	}
	constexpr Vec2_& operator*=( T rhs )
	{
		return *this = *this * rhs;
	}
	constexpr Vec2_ operator-( const Vec2_& rhs ) const
	{
		return Vec2_( x - rhs.x,y - rhs.y );
	}
	constexpr Vec2_& operator-=( const Vec2_& rhs )
	{
		return *this = *this - rhs;
	}
	constexpr Vec2_ operator/( T rhs ) const
	{
		return Vec2_{ x / rhs,y / rhs };
	}
	constexpr Vec2_& operator/=( T rhs )
	{
		*this = ( *this ) / rhs;
		return *this;
	}
	constexpr Vec2_ operator%( T rhs ) const
	{
		return Vec2_{ x % rhs,y % rhs };
	}
	constexpr Vec2_& operator%=( T rhs ) const
	{
		*this = ( *this ) % rhs;
		return( *this );
	}
	constexpr bool operator==( const Vec2_<T>& rhs )
	{
		return( x == rhs.x && y == rhs.y );
	}
	constexpr bool operator!=( const Vec2_<T>& rhs )
	{
		return( !( *this == rhs ) );
	}

	constexpr T GetLength() const
	{
		return T( std::sqrt( GetLengthSq() ) );
	}
	constexpr T GetLengthSq() const
	{
		return x * x + y * y;
	}
	constexpr T GetAngle() const
	{
		return T( atan2( y,x ) );
	}

	static constexpr Vec2_<T> FromAngle( float angle )
	{
		return( Vec2_<T>{ cos( angle ),sin( angle ) } );
	}

	constexpr Vec2_& Normalize()
	{
		return *this = GetNormalized();
	}
	constexpr Vec2_ GetNormalized() const
	{
		const T len = GetLength();
		if( len != T( 0.0f ) )
		{
			return *this * ( 1.0f / len );
		}
		return *this;
	}

	constexpr Vec2_ Rotation( const T angle ) const
	{
		Vec2_ result;
		result.x = x * cosf( angle ) - y * sinf( angle );
		result.y = x * sinf( angle ) + y * cosf( angle );
		return result;
	}

	static constexpr Vec2_ Up()
	{
		return Vec2_{ 0.0f,-1.0f };
	}
	static constexpr Vec2_ Down()
	{
		return Vec2_{ 0.0f,1.0f };
	}
	static constexpr Vec2_ Left()
	{
		return Vec2_{ -1.0f,0.0f };
	}
	static constexpr Vec2_ Right()
	{
		return Vec2_{ 1.0f,0.0f };
	}

	static constexpr float Lerp( float s,float e,float t )
	{
		return s + ( e - s ) * t;
	}
	static constexpr float Blerp( float c00,float c10,float c01,float c11,float tx,float ty )
	{
		return Lerp( Lerp( c00,c10,tx ),Lerp( c01,c11,tx ),ty );
	}
public:
	T x;
	T y;
};

typedef Vec2_<float> Vec2;
typedef Vec2_<int> Vei2;
typedef Vec2_<double> Ved2;