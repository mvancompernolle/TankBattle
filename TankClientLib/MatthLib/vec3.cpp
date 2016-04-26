#include "vec3.h"

namespace matth {

	const float& vec3::operator[]( int pos ) const {
		return v[pos];
	}

	float& vec3::operator[]( int pos ) {
		return v[pos];
	}

	vec3 vec3::operator-() const {
		return{ -x, -y, -z };
	}

	vec3 vec3::reflect( const vec3& n ) const {
		return{ *this - 2 * ( dot( *this, n.normal() ) * n.normal() ) };
	}

	float vec3::length() const {
		return sqrt( x*x + y*y + z*z );
	}

	vec3 vec3::normal() const {
		float len = length();
		return{ x / len, y / len, z / len };
	}

	float dot( const vec3& lhs, const vec3& rhs ) {
		return{lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z};
	}

	vec3 cross( const vec3& lhs, const vec3& rhs ) {
		return{ lhs.y * rhs.z - lhs.z * rhs.y, lhs.y * rhs.x - lhs.x * rhs.z , lhs.x * rhs.y - lhs.y * rhs.x };
	}

	vec3 interp( const vec3& lhs, const vec3& rhs, float val ) {
		val = std::fmax( 0.0f, std::fmin( val, 1.0f ) );
		return{ lhs * val + ( rhs * ( 1.0f - val ) ) };
	}

	// boolean operators
	bool operator==( const vec3& lhs, const vec3& rhs ) {
		return abs( lhs.x - rhs.x ) < FLT_EPSILON && abs( lhs.y - rhs.y ) < FLT_EPSILON  && abs( lhs.z - rhs.z ) < FLT_EPSILON;
	}

	bool operator!=( const vec3& lhs, const vec3& rhs ) {
		return !( lhs == rhs );
	}

	bool operator<( const vec3& lhs, const vec3& rhs ) {
		return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z;
	}

	bool operator<=( const vec3& lhs, const vec3& rhs ) {
		return ( lhs.x < rhs.x || abs( lhs.x - rhs.x ) < FLT_EPSILON ) && ( lhs.y < rhs.y || abs( lhs.y - rhs.y ) < FLT_EPSILON )
			&& ( lhs.z < rhs.z || abs( lhs.z - rhs.z ) < FLT_EPSILON );
	}

	bool operator>( const vec3& lhs, const vec3& rhs ) {
		return lhs.x > rhs.x && lhs.y > rhs.y && lhs.z > rhs.z;
	}

	bool operator>=( const vec3& lhs, const vec3& rhs ) {
		return ( lhs.x > rhs.x || abs( lhs.x - rhs.x ) < FLT_EPSILON ) && ( lhs.y > rhs.y || abs( lhs.y - rhs.y ) < FLT_EPSILON )
			&& ( lhs.z > rhs.z || abs( lhs.z - rhs.z ) < FLT_EPSILON );
	}

	// vector math
	vec3 operator+( const vec3& lhs, const vec3& rhs ) {
		return{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
	}

	vec3 operator-( const vec3& lhs, const vec3& rhs ) {
		return{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
	}

	vec3& operator+=( vec3& lhs, const vec3& rhs ) {
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
		return lhs;
	}

	vec3& operator-=( vec3& lhs, const vec3& rhs ) {
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
		return lhs;
	}

	// scalar math
	vec3 operator*( const vec3& lhs, float val ) {
		return{ lhs.x * val, lhs.y * val, lhs.z * val };
	}

	vec3 operator*( float val, const vec3& lhs ) {
		return lhs * val;
	}

	vec3 operator/( const vec3& lhs, float val ) {
		return{ lhs.x / val, lhs.y / val, lhs.z / val };
	}

	vec3& operator*=( vec3& lhs, float val ) {
		lhs.x *= val;
		lhs.y *= val;
		lhs.z *= val;
		return lhs;
	}

	vec3& operator/=( vec3& lhs, float val ) {
		lhs.x /= val;
		lhs.y /= val;
		lhs.z /= val;
		return lhs;
	}

}

