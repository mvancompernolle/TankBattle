#pragma once
#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <cfloat>

namespace matth {

#define PI 3.14159265359f

	struct vec2 {
		union {
			struct { float x, y; };
			float v[2];
		};

		static vec2 fromAngle( float angle );

		// offset opeartors
		const float& operator[]( int pos ) const;
		float& operator[]( int pos );

		vec2 operator-() const;
		vec2 reflect( const vec2& n ) const;
		float length() const;
		vec2 normal() const;
		vec2 perp() const;
		float angle() const;
	};

	float dot( const vec2& lhs, const vec2& rhs );
	float cross( const vec2& lhs, const vec2& rhs );
	vec2 projection( const vec2& lhs, const vec2& rhs );
	vec2 lerp( const vec2& lhs, const vec2& rhs, float val );

	// boolean operators
	bool operator==( const vec2& lhs, const vec2& rhs );
	bool operator!=( const vec2& lhs, const vec2& rhs );
	bool operator<( const vec2& lhs, const vec2& rhs );
	bool operator<=( const vec2& lhs, const vec2& rhs );
	bool operator>( const vec2& lhs, const vec2& rhs );
	bool operator>=( const vec2& lhs, const vec2& rhs );

	// vector math
	vec2 operator+( const vec2& lhs, const vec2& rhs );
	vec2 operator-( const vec2& lhs, const vec2& rhs );
	vec2 operator+=( vec2& lhs, const vec2& rhs );
	vec2 operator-=( vec2& lhs, const vec2& rhs );
	// scalar math
	vec2 operator*( const vec2& lhs, float val );
	vec2 operator*( float val, const vec2& lhs );
	vec2 operator/( const vec2& lhs, float val );
	vec2 operator*=( vec2& lhs, float val );
	vec2 operator/=( vec2& lhs, float val );
};


#endif // VEC2_H
