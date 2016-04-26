#pragma once
#ifndef COLLISION_H
#define COLLISION_H

#include "CollisionShapes.h"

namespace matth {

	struct CollisionData {
		bool wasCollision;
		vec2 normal;
		float depth;

		CollisionData( bool c = false, const vec2& n = { 0.0f, 0.0f }, float d = 0.0f )
			: wasCollision(c), normal( n ), depth( d ) {
		}
	};

	float pointPlanDist( const vec2& point, const Plane& plane );
	float RayPlaneDist( const Ray& ray, const Plane& plane );

	CollisionData collisionTest( const ConvexHull& a, const ConvexHull& b );
	CollisionData collisionTest( const ConvexHull& a, const Circle& b );
	inline CollisionData collisionTest( const Circle& a, const ConvexHull& b ) { return collisionTest( b, a ); }
	CollisionData collisionTest( const ConvexHull& a, const Ray& b );
	inline CollisionData collisionTest( const Ray& a, const ConvexHull& b ) { return collisionTest( b, a ); }
	CollisionData collisionTest( const ConvexHull& a, const Plane& b );
	inline CollisionData collisionTest( const Plane& a, const ConvexHull& b ) { return collisionTest( b, a ); }
	CollisionData collisionTest( const ConvexHull& a, const AABB& b );
	inline CollisionData collisionTest( const AABB& a, const ConvexHull& b ) { return collisionTest( b, a ); }

	CollisionData collisionTest( const AABB& a, const AABB& b );
	CollisionData collisionTest( const Circle& a, const AABB& b );
	inline CollisionData collisionTest( const AABB& a, const Circle& b ) { return collisionTest( b, a ); }
	CollisionData collisionTest( const AABB& a, const Plane& b );
	inline CollisionData collisionTest( const Plane& a, const AABB& b ) { return collisionTest( b, a ); }
	CollisionData collisionTest( const AABB& a, const Ray& b );
	inline CollisionData collisionTest( const Ray& a, const AABB& b ) { return collisionTest( b, a ); }
	CollisionData collisionTest( const Circle& a, const Circle& b );
	CollisionData collisionTest( const Circle& a, const Plane& b );
	inline CollisionData collisionTest( const Plane& a, const Circle& b ) { return collisionTest( b, a ); }
	CollisionData collisionTest( const Circle& a, const Ray& b );
	inline CollisionData collisionTest( const Ray& a, const Circle& b ) { return collisionTest( b, a ); }
	CollisionData collisionTest( const Ray& a, const Ray& b );
	CollisionData collisionTest( const Ray& a, const Plane& b );
	inline CollisionData collisionTest( const Plane& a, const Ray& b ) { return collisionTest( b, a ); }
	CollisionData collisionTest( const Plane& a, const Plane& b );
}


#endif // COLLISION_H