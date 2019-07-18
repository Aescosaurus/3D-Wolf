#pragma once

#include "Vec2.h"

template<typename T1,typename T2>
class Vec3_
	:
	public Vec2_<T1>
{
public:
	Vec3_( T1 x,T1 y,T2 z )
		:
		Vec2_( x,y ),
		z( z )
	{}
public:
	T2 z;
};

typedef Vec3_<float,float> Vec3;
typedef Vec3_<float,bool> Vec3b;