#pragma once
#ifndef VEC3_H
#define VEC3_H

#include "vec2.h"

namespace matth {

	struct vec3 {
		union {
			struct { float x, y, z; };
			struct { float r, g, b; };
			vec2 xy;
			float v[3];
		};
		//vec3(const vec2& xy, float z) : xy(xy), z(z) {}
		//vec3( float x = 0.0f, float y = 0.0f, float z = 0.0f ) : x( x ), y( y ), z( z ) {}

		// offset opeartors
		const float& operator[]( int pos ) const;
		float& operator[]( int pos );


		vec3 operator-() const;
		vec3 reflect( const vec3& n ) const;
		float length() const;
		vec3 normal() const;
	};

	float dot( const vec3& lhs, const vec3& rhs );
	vec3 cross( const vec3& lhs, const vec3& rhs );
	vec3 interp( const vec3& lhs, const vec3& rhs, float val );

	// boolean operators
	bool operator==( const vec3& lhs, const vec3& rhs );
	bool operator!=( const vec3& lhs, const vec3& rhs );
	bool operator<( const vec3& lhs, const vec3& rhs );
	bool operator<=( const vec3& lhs, const vec3& rhs );
	bool operator>( const vec3& lhs, const vec3& rhs );
	bool operator>=( const vec3& lhs, const vec3& rhs );

	// vector math
	vec3 operator+( const vec3& lhs, const vec3& rhs );
	vec3 operator-( const vec3& lhs, const vec3& rhs );
	vec3& operator+=( vec3& lhs, const vec3& rhs );
	vec3& operator-=( vec3& lhs, const vec3& rhs );
	// scalar math
	vec3 operator*( const vec3& lhs, float val );
	vec3 operator*( float val, const vec3& lhs );
	vec3 operator/( const vec3& lhs, float val );
	vec3& operator*=( vec3& lhs, float val );
	vec3& operator/=( vec3& lhs, float val );

}


#endif // VEC3_H
