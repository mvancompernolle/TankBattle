#include "mat2.h"

namespace matth {

	const vec2& mat2::operator[]( int pos ) const {
		return c[pos];
	}

	vec2& mat2::operator[]( int pos ) {
		return c[pos];
	}

	mat2 mat2::inverse() const {
		mat2 matrix = mat2::zero();
		float det = determinant();
		if ( det != 0.0f ) {
			matrix[0] = { c[1][1], -c[0][1] };
			matrix[1] = { -c[1][0], c[0][0] };
			return ( (1.0f / det) * matrix );
		}
		return matrix;
	}

	mat2 mat2::transpose() const {
		mat2 mat;
		for ( int i = 0; i < 2; ++i ) {
			for ( int j = 0; j < 2; ++j ) {
				mat[i][j] = c[j][i];
			}
		}
		return mat;
	}

	float mat2::determinant() const {
		return c[0][0] * c[1][1] - c[0][1] * c[1][0];
	}

	mat2::operator float*( ) {
		return (float*)c;
	}

	// static functions ///////////////////////////
	mat2 mat2::identity() {
		return{ 1, 0, 0, 1 };
	}

	mat2 mat2::zero() {
		return{ 0, 0, 0, 0 };
	}

	bool operator==( const mat2& lhs, const mat2& rhs ) {
		return lhs[0] == rhs[0] && lhs[1] == rhs[1];
	}

	bool operator!=( const mat2& lhs, const mat2& rhs ) {
		return !( lhs == rhs );
	}

	mat2 operator+( const mat2& lhs, const mat2& rhs ) {
		return mat2( lhs ) += rhs;
	}

	mat2 operator-( const mat2& lhs, const mat2& rhs ) {
		return mat2( lhs ) -= rhs;
	}

	mat2& operator+=( mat2& lhs, const mat2& rhs ) {
		for ( int i = 0; i < 2; ++i ) {
			for ( int j = 0; j < 2; ++j ) {
				lhs[i][j] += rhs[i][j];
			}
		}
		return lhs;
	}

	mat2& operator-=( mat2& lhs, const mat2& rhs ) {
		for ( int i = 0; i < 2; ++i ) {
			for ( int j = 0; j < 2; ++j ) {
				lhs[i][j] -= rhs[i][j];
			}
		}
		return lhs;
	}

	mat2 operator*( const mat2& lhs, float val ) {
		return mat2( lhs ) *= val;
	}

	mat2 operator*( float val, const mat2& rhs ) {
		return mat2( rhs ) *= val;
	}

	mat2& operator*=( mat2& lhs, float val ) {
		for ( int i = 0; i < 2; ++i ) {
			for ( int j = 0; j < 2; ++j ) {
				lhs[i][j] *= val;
			}
		}
		return lhs;
	}

	mat2& operator*=( float val, mat2& rhs ) {
		return rhs * val;
	}

	mat2 operator*( const mat2& lhs, const mat2& rhs ) {
		return mat2( lhs ) *= rhs;
	}

	mat2& operator*=( mat2& lhs, const mat2& rhs ) {
		mat2 trans = lhs.transpose();
		lhs[0] = { dot( trans[0], rhs[0] ), dot( trans[1], rhs[0] ) };
		lhs[1] = { dot( trans[0], rhs[1] ), dot( trans[1], rhs[1] ) };
		return lhs;
	}

	vec2 operator*( const mat2& lhs, const vec2& rhs ) {
		mat2 trans = lhs.transpose();
		return{ dot( trans[0], rhs ), dot( trans[1], rhs ) };
	}

	std::ostream& operator<<( std::ostream& os, const mat2& rhs ) {
		for ( int i = 0; i < 2; ++i ) {
			for ( int j = 0; j < 2; ++j ) {
				os << rhs.c[j][i] << " ";
			}
			os << std::endl;
		}
		return os;
	}
}


