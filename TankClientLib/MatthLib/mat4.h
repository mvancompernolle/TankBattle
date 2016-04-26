#pragma once
#ifndef MAT4_H
#define MAT4_H

#include "vec4.h"
#include "mat3.h"
#include "mat2.h"
#include <iostream>

namespace matth {

	_declspec( align( 32 ) ) struct mat4 {

		union {
			vec4 c[4];
			struct {
				union { vec4 c1; vec3 right; };
				union { vec4 c2; vec3 up; };
				union { vec4 c3; vec3 forward; };
				union { vec4 c4; vec3 pos; };
			};

		};
		void setBlock2x2( int row, int col, mat2 a );
		mat2 getBlock2x2( int row, int col ) const;

		const vec4& operator[]( int pos ) const;
		vec4& operator[]( int pos );
		operator mat3();
		mat4 inverse() const;
		mat4 transpose() const;
		float determinant() const;
		operator float*( );

		static mat4 identity();
		static mat4 zero();
		static mat4 rotation( const vec3& rotationAxis, float degrees );
		static mat4 translation( float x, float y, float z );
		static mat4 translation( const vec3& trans );
		static mat4 scale( float x, float y, float z );
		static mat4 scale( const vec3& s );
		static mat4 ortho( float l, float r, float b, float t, float n, float f );
		static mat4 frustum( float l, float r, float b, float t, float n, float f );
		static mat4 perspective( float fov, float aspect, float n, float f );
		static mat4 lookAt( const vec3& eye, const vec3& target, const vec3& up );
		static mat4 mat3ToMat4( float* mat3, float z = 1.0f );
	};

	bool operator==( const mat4& lhs, const mat4& rhs );
	bool operator!=( const mat4& lhs, const mat4& rhs );

	mat4 operator+( const mat4& lhs, const mat4& rhs );
	mat4 operator-( const mat4& lhs, const mat4& rhs );
	mat4& operator+=( mat4& lhs, const mat4& rhs );
	mat4& operator-=( mat4& lhs, const mat4& rhs );
	mat4 operator*( const mat4& lhs, float val );
	mat4 operator*( float val, const mat4& rhs );
	mat4& operator*=( mat4& lhs, float val );
	mat4& operator*=( float val, mat4& rhs );
	mat4 operator*( const mat4& lhs, const mat4& rhs );
	mat4& operator*=( mat4& lhs, const mat4& rhs );
	vec4 operator*( const mat4& lhs, const vec4& rhs );
	std::ostream& operator<<( std::ostream& os, const mat4& rhs );
};


#endif // MAT4_H