
#pragma once
#ifndef COLLISION_SHAPES_H
#define COLLISION_SHAPES_H

#include "vec2.h"
#include "mat3.h"
#include <vector>

namespace matth {

	struct ConvexHull {
		std::vector<vec2> verts;
	};

	struct Ray {
		vec2 pos, dir;
		float len;
	};

	struct Plane {
		vec2 pos, normal;
	};

	struct Circle {
		vec2 pos;
		float radius;
	};

	struct AABB {
		vec2 pos;
		vec2 hExtents;
		vec2 min() const {
			return pos - hExtents;
		}
		vec2 max() const {
			return pos + hExtents;
		}
	};

	Circle operator*( const mat3& m, const Circle& a );
	AABB operator*( const mat3& m, const AABB& a );
	Ray operator*( const mat3& m, const Ray& a );
	Plane operator*( const mat3& m, const Plane& a );
	ConvexHull operator*( const mat3& m, const ConvexHull& a );
}

#endif // COLLISION_SHAPES_H
