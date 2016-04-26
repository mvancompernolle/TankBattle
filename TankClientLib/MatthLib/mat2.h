#pragma once
#include "vec2.h"
#include <iostream>

namespace matth {

	struct mat2 {
		vec2 c[2];

		const vec2& operator[]( int pos ) const;
		vec2& operator[]( int pos );
		mat2 inverse() const;
		mat2 transpose() const;
		float determinant() const;
		operator float*( );

		static mat2 identity();
		static mat2 zero();

	};

	bool operator==( const mat2& lhs, const mat2& rhs );
	bool operator!=( const mat2& lhs, const mat2& rhs );

	mat2 operator+( const mat2& lhs, const mat2& rhs );
	mat2 operator-( const mat2& lhs, const mat2& rhs );
	mat2& operator+=( mat2& lhs, const mat2& rhs );
	mat2& operator-=( mat2& lhs, const mat2& rhs );
	mat2 operator*( const mat2& lhs, float val );
	mat2 operator*( float val, const mat2& rhs );
	mat2& operator*=( mat2& lhs, float val );
	mat2& operator*=( float val, mat2& rhs );
	mat2 operator*( const mat2& lhs, const mat2& rhs );
	mat2& operator*=( mat2& lhs, const mat2& rhs );
	vec2 operator*( const mat2& lhs, const vec2& rhs );
	std::ostream& operator<<( std::ostream& os, const mat2& rhs );
};

