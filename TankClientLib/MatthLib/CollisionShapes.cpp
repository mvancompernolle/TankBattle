#include "CollisionShapes.h"
#include "collision.h"

namespace matth {

	Circle operator*( const mat3& m, const Circle& a ) {
		Circle circle;
		circle.pos = ( m * vec3{ a.pos.x, a.pos.y, 1.0f } ).xy;
		circle.radius = fmaxf( ( m * vec3{ a.radius, 0.0f, 0.0f } ).length(), ( m * vec3{ 0.0f, a.radius, 0.0f } ).length() );
		return circle;
	}

	AABB operator*( const mat3& m, const AABB& a ) {
		AABB aabb;
		vec2 oMin = a.min();
		vec2 oMax = a.max();
		vec2 rMin = m[2].xy;
		vec2 rMax = m[2].xy;
		float p, q;

		for ( size_t j = 0; j < 2; ++j ) {
			for ( size_t i = 0; i < 2; ++i ) {
				p = oMin[i] * m[i][j];
				q = oMax[i] * m[i][j];
				if ( p < q ) std::swap( p, q );

				rMin[j] += p;
				rMax[j] += q;
			}
		}
		aabb.pos = { ( rMin + rMax ) / 2.0f };
		aabb.hExtents = { ( rMin - rMax ) / 2.0f };
		return aabb;
	}
	Ray operator*( const mat3& m, const Ray& a ) {
		Ray ray;
		vec3 dir{ a.dir.x, a.dir.y, 0.0f };
		vec3 pos{ a.pos.x, a.pos.y, 1.0f };
		dir = dir * a.len;
		ray = { ( m * pos ).xy, ( m * dir ).xy };
		ray.len = ray.dir.length();
		ray.dir = ray.dir.normal();
		return ray;
	}
	Plane operator*( const mat3& m, const Plane& a ) {
		const vec3 nor{ a.normal.x, a.normal.y, 0.0f };
		const vec3 pos{ a.pos.x, a.pos.y, 1.0f };
		return{ ( m * pos ).xy, ( m*nor ).normal().xy };
	}
	ConvexHull operator*( const mat3& m, const ConvexHull& a ) {
		ConvexHull hull;
		for ( const vec2& vertex : a.verts ) {
			hull.verts.push_back( ( m * vec3{ vertex.x, vertex.y, 1.0f } ).xy );
		}
		return hull;
	}
}
