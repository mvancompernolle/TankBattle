#pragma once

#include "mat4.h"

#define PI 3.14159265359f
#define DEGTORAD PI / 180.0f
#define RADTODEG 180.0f / PI

namespace matth {

	inline vec2 hermite( const vec2& a, const vec2& b, const vec2& tanA, const vec2& tanB, float t) {
		const float h1 = pow( 2 * t, 3 ) - pow( 3 * t, 2 ) + 1;
		const float h2 = pow( -2 * t, 3 ) + pow( 3 * t, 2 );
		const float h3 = pow( t, 3 ) - 2 * pow( t, 2 ) + t;
		const float h4 = pow( t, 3 ) - pow( t, 2 );

		return{h1 * a.x + h2 * b.x + h3 * tanA.x + h4 * tanB.x, h1 * a.y + h2 * b.y + h3 * tanA.y + h4 * tanB.y};
	}
	
	inline vec2 bezier( const vec2& a, const vec2& b, const vec2& c, const vec2& d, float t ) {
		const vec2 ab = lerp( a, b, t );
		const vec2 bc = lerp( b, c, t );
		const vec2 cd = lerp( c, d, t );
		return lerp(lerp(ab, bc, t), lerp(bc, cd, t), t);
	}

	template<class T>
	inline T min( const T& a, const T& b ) {
		return ( a < b ? a : b );
	}
	template<class T>
	inline T max( const T& a, const T& b ) {
		return ( a > b ? a : b );
	}
	template<class T>
	inline T clamp( const T& a, const T& min, const T& max ) {
		if ( a < min ) return min;
		if ( a > max ) return max;
		return a;
	}
}