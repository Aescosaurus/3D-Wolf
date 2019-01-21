#pragma once

namespace chili
{
	static constexpr float pi = 3.14159265f;
	static constexpr float deg2rad( float deg )
	{
		return( deg * ( pi / 180.0f ) );
	}
}