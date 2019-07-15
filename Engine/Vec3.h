#pragma once

#include "Vec2.h"

template<typename T>
class Vec3_
	:
	public Vec2_<T>
{
public:
	Vec3_( T x,T y,T z )
		:
		Vec2_( x,y ),
		z( z )
	{}
public:
	T z;
};

typedef Vec3_<float> Vec3;
