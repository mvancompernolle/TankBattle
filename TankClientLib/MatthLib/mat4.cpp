#include "mat4.h"

namespace matth {

	const vec4& mat4::operator[]( int pos ) const {
		return c[pos];
	}

	vec4& mat4::operator[]( int pos ) {
		return c[pos];
	}

	void mat4::setBlock2x2( int row, int col, mat2 a ) {
		for ( int i = 0; i < 2; ++i ) {
			for ( int j = 0; j < 2; ++j ) {
				c[i + col][j + row] = a[i][j];
			}
		}
	}

	mat2 mat4::getBlock2x2( int row, int col ) const {
		mat2 matrix;
		for ( int i = 0; i < 2; ++i ) {
			for ( int j = 0; j < 2; ++j ) {
				matrix[i][j] = c[i + col][j + row];
			}
		}
		return matrix;
	}

	mat4::operator float*( ) {
		return (float*)c;
	}

	mat4 mat4::inverse() const {
		const mat2 a = getBlock2x2( 0, 0 );
		const mat2 b = getBlock2x2( 0, 2 );
		const mat2 c = getBlock2x2( 2, 0 );
		const mat2 d = getBlock2x2( 2, 2 );
		const mat2 ai = a.inverse();
		const mat2 ca = c * ai;
		const mat2 ab = ai * b;
		const mat2 dcab = ( d - ca *b ).inverse();

		mat4 matrix;
		matrix.setBlock2x2( 0, 0, mat2{ ai + ab * dcab * ca } );
		matrix.setBlock2x2( 0, 2, mat2{ ( ab * -1 )*dcab } );
		matrix.setBlock2x2( 2, 0, mat2{ ( dcab * -1 ) * ca } );
		matrix.setBlock2x2( 2, 2, mat2{ dcab } );
		return matrix;
	}

	mat4 mat4::transpose() const {
		mat4 mat;
		for ( int i = 0; i < 4; ++i ) {
			for ( int j = 0; j < 4; ++j ) {
				mat[i][j] = c[j][i];
			}
		}
		return mat;
	}

	float mat4::determinant() const {
		const mat3 matrix1 = {
			c[1][1], c[1][2], c[1][3],
			c[2][1], c[2][2], c[2][3],
			c[3][1], c[3][2], c[3][3],
		};
		const mat3 matrix2 = {
			c[0][1], c[0][2], c[0][3],
			c[2][1], c[2][2], c[2][3],
			c[3][1], c[3][2], c[3][3],
		};
		const mat3 matrix3 = {
			c[0][1], c[0][2], c[0][3],
			c[1][1], c[1][2], c[1][3],
			c[3][1], c[3][2], c[3][3],
		};
		const mat3 matrix4 = {
			c[0][1], c[0][2], c[0][3],
			c[1][1], c[1][2], c[1][3],
			c[2][1], c[2][2], c[2][3],
		};
		return ( c[0][0] * matrix1.determinant() ) - ( c[1][0] * matrix2.determinant() )
			+ ( c[2][0] * matrix3.determinant() ) - ( c[3][0] * matrix4.determinant() );
	}

	mat4::operator mat3() {
		return{ c[0][1], c[0][1], c[0][2],
			c[1][0], c[1][1], c[1][2],
			c[2][0], c[2][1], c[2][2] };
	}

	mat4 mat4::identity() {
		return{ 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f, };
	}

	mat4 mat4::zero() {
		return{ 0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f, };
	}

	mat4 mat4::rotation( const vec3& axis, float degrees ) {
		const float x2 = axis.x * axis.x, y2 = axis.y * axis.y, z2 = axis.z * axis.z;
		const float L = x2 + y2 + z2;
		const float rads = degrees * ( PI / 180.0f );
		mat4 matrix = identity();

		matrix[0][0] = ( x2 + ( y2 + z2 ) * cos( rads ) ) / L;
		matrix[0][1] = ( axis.x * axis.y * ( 1 - cos( rads ) ) + axis.z * sqrt( L ) * sin( rads ) ) / L;
		matrix[0][2] = ( axis.x * axis.z * ( 1 - cos( rads ) ) + axis.y * sqrt( L ) * sin( rads ) ) / L;

		matrix[1][0] = ( axis.x * axis.y * ( 1 - cos( rads ) ) - axis.z * sqrt( L ) * sin( rads ) ) / L;
		matrix[1][1] = ( y2 + ( x2 + z2 ) * cos( rads ) ) / L;
		matrix[1][2] = ( axis.y * axis.z * ( 1 - cos( rads ) ) + axis.x * sqrt( L ) * sin( rads ) ) / L;

		matrix[2][0] = ( axis.x * axis.z * ( 1 - cos( rads ) ) - axis.y * sqrt( L ) * sin( rads ) ) / L;
		matrix[2][1] = ( axis.x * axis.y * ( 1 - cos( rads ) ) + axis.z * sqrt( L ) * sin( rads ) ) / L;
		matrix[2][2] = ( z2 + ( x2 + y2 ) * cos( rads ) ) / L;

		return matrix;
	}

	mat4 mat4::translation( float x, float y, float z ) {
		mat4 mat = identity();
		mat[3][0] = x;
		mat[3][1] = y;
		mat[3][2] = z;
		return mat;
	}

	mat4 mat4::translation( const vec3& trans ) {
		return translation( trans.x, trans.y, trans.z );
	}

	mat4 mat4::scale( float x, float y, float z ) {
		return{
			x,		0.0f,	0.0f,	0.0f,
			0.0f,	y,		0.0f,	0.0f,
			0.0f,	0.0f,	z,		0.0f,
			0.0f,	0.0f,	0.0f,	1.0f
		};
	}

	mat4 mat4::scale( const vec3& s ) {
		return scale( s.x, s.y, s.z );
	}

	mat4 mat4::ortho( float l, float r, float b, float t, float n, float f ) {
		mat4 matrix = identity();
		matrix[0] = { 2.0f / ( r - l ), 0.0f, 0.0f, 0.0f };
		matrix[1] = { 0.0f, 2.0f / ( t - b ), 0.0f, 0.0f };
		matrix[2] = { 0.0f, 0.0f, -2.0f / ( f - n ), 0.0f };
		matrix[3] = { -( r + l ) / ( r - l ), -( t + b ) / ( t - b ), -( f + n ) / ( f - n ), 1.0f };
		return matrix;
	}

	mat4 mat4::frustum( float l, float r, float b, float t, float n, float f ) {
		const float ral = r + l;
		const float rsl = r - l;
		const float tsb = r - b;
		const float tab = r + b;
		const float fan = f + n;
		const float fsn = f - n;

		return mat4{
			2.0f * n / rsl, 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f * n / tsb, 0.0f, 0.0f,
			ral / rsl, tab / tsb, -fan / fsn, -1.0f,
			0.0f, 0.0f, ( -2.0f * f * n ) / fsn, 0.0f
		};
	}

	mat4 mat4::perspective( float fov, float aspect, float n, float f ) {
		const float cotan = 1.0f / tanf( ( fov * 180.0f / PI ) / 2.0f );

		return mat4{
			cotan / aspect, 0.0f, 0.0f, 0.0f,
			0.0f, cotan, 0.0f, 0.0f,
			0.0f, 0.0f, ( f + n ) / ( n - f ), -1.0f,
			0.0f, 0.0f, ( 2.0f * f * n ) / ( n - f ), 0.0f
		};
	}

	mat4 mat4::lookAt( const vec3& eye, const vec3& target, const vec3& up ) {
		const vec3 n = ( eye + target ).normal();
		const vec3 u = cross( up, n ).normal();
		const vec3 v = cross( n, u );

		return{
			u[0], v[0], n[0], 0.0f,
			u[1], v[1], n[1], 0.0f,
			u[2], v[2], n[2], 0.0f,
			dot( -u, eye ), dot( -v, eye ), dot( -n, eye ), 1.0f
		};
	}

	mat4 mat4::mat3ToMat4( float* mat3, float z ) {
		return{ mat3[0], mat3[1], 0.0f, mat3[2],
				mat3[3], mat3[4], 0.0f, mat3[5],
				0.0f, 0.0f, 1.0f, 0.0f,
				mat3[6], mat3[7], z, mat3[8] };
	}

	bool operator==( const mat4& lhs, const mat4& rhs ) {
		return lhs[0] == rhs[0] && lhs[1] == rhs[1] && lhs[2] == rhs[2] && lhs[3] == rhs[3];
	}

	bool operator!=( const mat4& lhs, const mat4& rhs ) {
		return !( lhs == rhs );
	}

	mat4 operator+( const mat4& lhs, const mat4& rhs ) {
		return mat4( lhs ) += rhs;
	}

	mat4 operator-( const mat4& lhs, const mat4& rhs ) {
		return mat4( lhs ) -= rhs;
	}

	mat4& operator+=( mat4& lhs, const mat4& rhs ) {
		for ( int i = 0; i < 4; ++i ) {
			for ( int j = 0; j < 4; ++j ) {
				lhs[i][j] += rhs[i][j];
			}
		}
		return lhs;
	}

	mat4& operator-=( mat4& lhs, const mat4& rhs ) {
		for ( int i = 0; i < 4; ++i ) {
			for ( int j = 0; j < 4; ++j ) {
				lhs[i][j] -= rhs[i][j];
			}
		}
		return lhs;
	}

	mat4 operator*( const mat4& lhs, float val ) {
		return mat4( lhs ) *= val;
	}

	mat4 operator*( float val, const mat4& rhs ) {
		return mat4( rhs ) *= val;
	}

	mat4& operator*=( mat4& lhs, float val ) {
		for ( int i = 0; i < 4; ++i ) {
			for ( int j = 0; j < 4; ++j ) {
				lhs[i][j] *= val;
			}
		}
		return lhs;
	}

	mat4& operator*=( float val, mat4& rhs ) {
		return rhs *= val;
	}

	mat4 operator*( const mat4& lhs, const mat4& rhs ) {
		return mat4( lhs ) *= rhs;
	}

	mat4& operator*=( mat4& lhs, const mat4& rhs ) {
		mat4 trans = lhs.transpose();
		lhs = { dot( trans[0], rhs[0] ),
				dot( trans[1], rhs[0] ),
				dot( trans[2], rhs[0] ),
				dot( trans[3], rhs[0] ),

				dot( trans[0], rhs[1] ),
				dot( trans[1], rhs[1] ),
				dot( trans[2], rhs[1] ),
				dot( trans[3], rhs[1] ),

				dot( trans[0], rhs[2] ),
				dot( trans[1], rhs[2] ),
				dot( trans[2], rhs[2] ),
				dot( trans[3], rhs[2] ),

				dot( trans[0], rhs[3] ),
				dot( trans[1], rhs[3] ),
				dot( trans[2], rhs[3] ),
				dot( trans[3], rhs[3] ) };
		return lhs;
	}

	vec4 operator*( const mat4& lhs, const vec4& rhs ) {
		mat4 trans = lhs.transpose();
		return{ dot( trans[0], rhs ), dot( trans[1], rhs ), dot( trans[2], rhs ), dot( trans[3], rhs ) };
	}

	std::ostream& operator<<( std::ostream& os, const mat4& rhs ) {
		for ( int i = 0; i < 4; ++i ) {
			for ( int j = 0; j < 4; ++j ) {
				os << rhs.c[j][i] << " ";
			}
			os << std::endl;
		}
		return os;
	}

};
