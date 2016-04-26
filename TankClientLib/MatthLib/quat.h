#pragma once
#ifndef	QUAT_H
#define QUAT_H

#include "mat3.h"

namespace matth {
	struct quat {
		union {
			float x, y, z, w;
			float v[4];
		};
	};

	/*quat makeRotate( const vec3& axis, float angle );
	mat3 quatToMat3( const quat& q );
	quat mat3ToQuat( const mat3& m );
	quat slerp( const quat& q1, const quat& q2, float t );*/
}


#endif // QUAT_H
