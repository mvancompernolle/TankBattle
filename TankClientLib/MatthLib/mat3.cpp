#include "mat3.h"

namespace matth {

	const vec3& mat3::operator[]( int pos ) const {
		return c[pos];
	}

	vec3& mat3::operator[]( int pos ) {
		return c[pos];
	}

	mat3 mat3::inverse() const {
		mat3 matrix = mat3::zero();
		float det = determinant();
		if ( det != 0.0f ) {
			matrix[0] = {c[1][1]*c[2][2] - c[2][1]*c[1][2], c[2][1]*c[0][2] - c[0][1]*c[2][2], c[0][1] * c[1][2] - c[1][1] * c[0][2]};
			matrix[1] = { c[2][0] * c[1][2] - c[1][0] * c[2][2], c[0][0] * c[2][2] - c[2][0] * c[0][2], c[1][0] * c[0][2] - c[0][0] * c[1][2] };
			matrix[2] = { c[1][0] * c[2][1] - c[2][0] * c[1][1], c[2][0] * c[0][1] - c[0][0] * c[2][1], c[0][0] * c[1][1] - c[1][0] * c[0][1] };
			return ( 1.0f / determinant() * matrix );
		}
		return matrix;

	}

	mat3 mat3::transpose() const {
		mat3 mat;
		for ( int i = 0; i < 3; ++i ) {
			for ( int j = 0; j < 3; ++j ) {
				mat[i][j] = c[j][i];
			}
		}
		return mat;
	}

	float mat3::determinant() const {
		return c[0][0] * ( c[1][1] * c[2][2] - c[1][2] * c[2][1] ) -
			c[1][0] * ( c[0][1] * c[2][2] - c[2][1] * c[0][2] ) +
			c[2][0] * ( c[0][1] * c[1][2] - c[1][1] * c[0][2] );

	}

	mat3::operator float*( ) {
		return (float*)c;
	}

	// static functions ///////////////////////////
	mat3 mat3::identity() {
		return{ 1, 0, 0,
				0, 1, 0,
				0, 0, 1 };
	}

	mat3 mat3::zero() {
		return{ 0, 0, 0,
				0, 0, 0,
				0, 0, 0 };
	}

	mat3 mat3::rotation( float degrees ) {
		float rads = degrees * ( PI / 180.0f );
		mat3 mat = identity();
		mat[0] = { cosf( rads ), sinf( rads ), 0.0f };
		mat[1] = { -sinf( rads ), cosf( rads ), 0.0f };
		return mat;
	}

	mat3 mat3::translation( float x, float y ) {
		mat3 mat = identity();
		mat[2][0] = x;
		mat[2][1] = y;
		return mat;
	}

	mat3 mat3::translation( const vec2& trans ) {
		return translation( trans.x, trans.y );
	}

	mat3 mat3::scale( float x, float y ) {
		mat3 mat = identity();
		mat[0][0] = x;
		mat[1][1] = y;
		return mat;
	}

	mat3 mat3::scale( const vec2& s ) {
		return scale( s.x, s.y );
	}

	bool operator==( const mat3& lhs, const mat3& rhs ) {
		return lhs[0] == rhs[0] && lhs[1] == rhs[1] && lhs[2] == rhs[2];
	}

	bool operator!=( const mat3& lhs, const mat3& rhs ) {
		return !( lhs == rhs );
	}

	mat3 operator+( const mat3& lhs, const mat3& rhs ) {
		return mat3( lhs ) += rhs;
	}

	mat3 operator-( const mat3& lhs, const mat3& rhs ) {
		return mat3( lhs ) -= rhs;
	}

	mat3& operator+=( mat3& lhs, const mat3& rhs ) {
		for ( int i = 0; i < 3; ++i ) {
			for ( int j = 0; j < 3; ++j ) {
				lhs[i][j] += rhs[i][j];
			}
		}
		return lhs;
	}

	mat3& operator-=( mat3& lhs, const mat3& rhs ) {
		for ( int i = 0; i < 3; ++i ) {
			for ( int j = 0; j < 3; ++j ) {
				lhs[i][j] -= rhs[i][j];
			}
		}
		return lhs;
	}

	mat3 operator*( const mat3& lhs, float val ) {
		return mat3( lhs ) *= val;
	}

	mat3 operator*( float val, const mat3& rhs ) {
		return mat3( rhs ) *= val;
	}

	mat3& operator*=( mat3& lhs, float val ) {
		for ( int i = 0; i < 3; ++i ) {
			for ( int j = 0; j < 3; ++j ) {
				lhs[i][j] *= val;
			}
		}
		return lhs;
	}

	mat3& operator*=( float val, mat3& rhs ) {
		return rhs * val;
	}

	mat3 operator*( const mat3& lhs, const mat3& rhs ) {
		return mat3( lhs ) *= rhs;
	}

	mat3& operator*=( mat3& lhs, const mat3& rhs ) {
		mat3 trans = lhs.transpose();
		lhs[0] = { dot( trans[0], rhs[0] ), dot( trans[1], rhs[0] ), dot( trans[2], rhs[0] ) };
		lhs[1] = { dot( trans[0], rhs[1] ), dot( trans[1], rhs[1] ), dot( trans[2], rhs[1] ) };
		lhs[2] = { dot( trans[0], rhs[2] ), dot( trans[1], rhs[2] ), dot( trans[2], rhs[2] ) };
		return lhs;
	}

	vec3 operator*( const mat3& lhs, const vec3& rhs ) {
		mat3 trans = lhs.transpose();
		return{ dot( trans[0], rhs ), dot( trans[1], rhs ),dot( trans[2], rhs ) };
	}

	std::ostream& operator<<( std::ostream& os, const mat3& rhs ) {
		for ( int i = 0; i < 3; ++i ) {
			for ( int j = 0; j < 3; ++j ) {
				os << rhs.c[j][i] << " ";
			}
			os << std::endl;
		}
		return os;
	}
};
