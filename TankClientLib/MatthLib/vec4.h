#pragma once
#ifndef VEC4_H
#define VEC4_H

#include "vec3.h"


namespace matth {

	struct vec4 {
		union {
			struct { float x, y, z, w; };
			struct { vec3 xyz; };
			struct { vec2 xy; };
			struct { float r, g, b, a; };
			struct { vec3 rgb; };
			float v[4];
		};

		// offset opeartors
		const float& operator[]( int pos ) const;
		float& operator[]( int pos );

		vec4 operator-() const;
		vec4 reflect( const vec4& n ) const;
		float length() const;
		vec4 normal() const;
	};

	float dot( const vec4& lhs, const vec4& rhs );
	vec4 interp( const vec4& lhs, const vec4& rhs, float val );

	// boolean operators
	bool operator==( const vec4& lhs, const vec4& rhs );
	bool operator!=( const vec4& lhs, const vec4& rhs );
	bool operator<( const vec4& lhs, const vec4& rhs );
	bool operator<=( const vec4& lhs, const vec4& rhs );
	bool operator>( const vec4& lhs, const vec4& rhs );
	bool operator>=( const vec4& lhs, const vec4& rhs );

	// vector math
	vec4 operator+( const vec4& lhs, const vec4& rhs );
	vec4 operator-( const vec4& lhs, const vec4& rhs );
	vec4& operator+=( vec4& lhs, const vec4& rhs );
	vec4& operator-=( vec4& lhs, const vec4& rhs );
	// scalar math
	vec4 operator*( const vec4& lhs, float val );
	vec4 operator*( float val, const vec4& lhs );
	vec4 operator/( const vec4& lhs, float val );
	vec4& operator*=( vec4& lhs, float val );
	vec4& operator/=( vec4& lhs, float val );
}


#endif // VEC4_H
