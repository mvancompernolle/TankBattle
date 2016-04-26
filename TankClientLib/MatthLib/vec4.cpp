#include "vec4.h"

namespace matth {

	const float& vec4::operator[]( int pos ) const {
		return v[pos];
	}

	float& vec4::operator[]( int pos ) {
		return v[pos];
	}

	vec4 vec4::operator-() const {
		return{ -x, -y, -z, -w };
	}

	vec4 vec4::reflect( const vec4& n ) const {
		return{ *this - 2 * ( dot( *this, n.normal() ) * n.normal() ) };
	}

	float vec4::length() const {
		return sqrt( x*x + y*y + z*z + w*w );
	}

	vec4 vec4::normal() const {
		float len = length();
		return{ x / len, y / len, z / len, w / len };
	}

	float dot( const vec4& lhs, const vec4& rhs ) {
		return{ lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w };
	}

	vec4 interp( const vec4& lhs, const vec4& rhs, float val ) {
		val = std::fmax( 0.0f, std::fmin( val, 1.0f ) );
		return{ lhs * val + ( rhs * ( 1.0f - val ) ) };
	}

	// boolean operators
	bool operator==( const vec4& lhs, const vec4& rhs ) {
		return abs( lhs.x - rhs.x ) < FLT_EPSILON && abs( lhs.y - rhs.y ) < FLT_EPSILON  && abs( lhs.z - rhs.z ) < FLT_EPSILON
			&& abs( lhs.w - rhs.w ) < FLT_EPSILON;
	}

	bool operator!=( const vec4& lhs, const vec4& rhs ) {
		return !( lhs == rhs );
	}

	bool operator<( const vec4& lhs, const vec4& rhs ) {
		return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z && lhs.w < rhs.w;
	}

	bool operator<=( const vec4& lhs, const vec4& rhs ) {
		return ( lhs.x < rhs.x || abs( lhs.x - rhs.x ) < FLT_EPSILON ) && ( lhs.y < rhs.y || abs( lhs.y - rhs.y ) < FLT_EPSILON )
			&& ( lhs.z < rhs.z || abs( lhs.z - rhs.z ) < FLT_EPSILON && ( lhs.w < rhs.w || abs( lhs.w - rhs.w ) < FLT_EPSILON ));
	}

	bool operator>( const vec4& lhs, const vec4& rhs ) {
		return lhs.x > rhs.x && lhs.y > rhs.y && lhs.z > rhs.z && lhs.w > rhs.w;
	}

	bool operator>=( const vec4& lhs, const vec4& rhs ) {
		return ( lhs.x > rhs.x || abs( lhs.x - rhs.x ) < FLT_EPSILON ) && ( lhs.y > rhs.y || abs( lhs.y - rhs.y ) < FLT_EPSILON )
			&& ( lhs.z > rhs.z || abs( lhs.z - rhs.z ) < FLT_EPSILON && ( lhs.w > rhs.w || abs( lhs.w - rhs.w ) < FLT_EPSILON ));
	}

	// vector math
	vec4 operator+( const vec4& lhs, const vec4& rhs ) {
		return{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
	}

	vec4 operator-( const vec4& lhs, const vec4& rhs ) {
		return{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
	}

	vec4& operator+=( vec4& lhs, const vec4& rhs ) {
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
		lhs.w += rhs.w;
		return lhs;
	}

	vec4& operator-=( vec4& lhs, const vec4& rhs ) {
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
		return lhs;
	}

	// scalar math
	vec4 operator*( const vec4& lhs, float val ) {
		return{ lhs.x * val, lhs.y * val, lhs.z * val, lhs.w * val };
	}

	vec4 operator*( float val, const vec4& lhs ) {
		return lhs * val;
	}

	vec4 operator/( const vec4& lhs, float val ) {
		return{ lhs.x / val, lhs.y / val, lhs.z / val, lhs.w / val };
	}

	vec4& operator*=( vec4& lhs, float val ) {
		lhs.x *= val;
		lhs.y *= val;
		lhs.z *= val;
		lhs.w *= val;
		return lhs;
	}

	vec4& operator/=( vec4& lhs, float val ) {
		lhs.x /= val;
		lhs.y /= val;
		lhs.z /= val;
		lhs.w /= val;
		return lhs;
	}

}