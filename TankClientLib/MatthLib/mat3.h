#pragma once
#ifndef MAT3_H
#define MAT3_H

#include "vec3.h"
#include <iostream>

namespace matth {

	struct mat3 {
		union {
			vec3 c[3];
			struct {
				union { vec3 c1; vec2 right; };
				union { vec3 c2; vec2 up; };
				union { vec3 c3; vec2 pos; };
			};
		};

		const vec3& operator[]( int pos ) const;
		vec3& operator[]( int pos );
		mat3 inverse() const;
		mat3 transpose() const;
		float determinant() const;
		operator float*( );

		static mat3 identity();
		static mat3 zero();
		static mat3 rotation( float degrees );
		static mat3 translation( float x, float y );
		static mat3 translation( const vec2& trans );
		static mat3 scale( float x, float y );
		static mat3 scale( const vec2& s );

	};

	bool operator==( const mat3& lhs, const mat3& rhs );
	bool operator!=( const mat3& lhs, const mat3& rhs );

	mat3 operator+( const mat3& lhs, const mat3& rhs );
	mat3 operator-( const mat3& lhs, const mat3& rhs );
	mat3& operator+=( mat3& lhs, const mat3& rhs );
	mat3& operator-=( mat3& lhs, const mat3& rhs );
	mat3 operator*( const mat3& lhs, float val );
	mat3 operator*( float val, const mat3& rhs );
	mat3& operator*=( mat3& lhs, float val );
	mat3& operator*=( float val, mat3& rhs );
	mat3 operator*( const mat3& lhs, const mat3& rhs );
	mat3& operator*=( mat3& lhs, const mat3& rhs );
	vec3 operator*( const mat3& lhs, const vec3& rhs );
	std::ostream& operator<<( std::ostream& os, const mat3& rhs );
};


#endif // MAT3_H
