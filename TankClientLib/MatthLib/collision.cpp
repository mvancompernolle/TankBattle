#include "collision.h"
#include "matth.h"
#include <vector>
#include <iostream>

namespace matth {

	float pointPlanDist( const vec2& point, const Plane& plane ) {
		return dot( plane.normal, ( point - plane.pos ) );
	}
	float RayPlaneDist( const Ray& ray, const Plane& plane ) {
		return dot( plane.normal, ray.pos - plane.pos ) / -dot( plane.normal, ray.dir );
	}

	CollisionData collisionTest( const ConvexHull& a, const ConvexHull& b ) {
		CollisionData cData;
		cData.depth = INFINITY;
		std::vector<vec2> axes;
		std::vector<float> pDepths;

		for ( int i = 0; i < a.verts.size(); ++i ) {
			vec2 vecBetween = a.verts[i] - a.verts[( i + 1 ) % a.verts.size()];
			axes.push_back( vecBetween.normal().perp() );
		}
		for ( int i = 0; i < b.verts.size(); ++i ) {
			vec2 vecBetween = b.verts[i] - b.verts[( i + 1 ) % b.verts.size()];
			axes.push_back( vecBetween.normal().perp() );
		}

		for ( int axis = 0; axis < axes.size(); ++axis ) {
			float aMin = INFINITY, aMax = -INFINITY;
			float bMin = INFINITY, bMax = -INFINITY;

			for ( int i = 0; i < a.verts.size(); ++i ) {
				float projection = dot( axes[axis], a.verts[i] );
				aMin = fminf( projection, aMin );
				aMax = fmaxf( projection, aMax );
			}
			for ( int i = 0; i < b.verts.size(); ++i ) {
				float projection = dot( axes[axis], b.verts[i] );
				bMin = fminf( projection, bMin );
				bMax = fmaxf( projection, bMax );
			}

			float pDepth = fminf( aMax - bMin, bMax - aMin );

			// handle case where there was a gap and no collision
			if ( pDepth < 0.0f ) {
				cData.wasCollision = false;
				if ( pDepth > cData.depth || cData.depth >= 0.0f ) {
					cData.depth = pDepth;
				}
				cData.normal = ( pDepth == ( aMax - bMin ) ? -axes[axis] : axes[axis] );
			}
			// handle case where there is overlap and no gap has been detected yet
			else if ( pDepth >= 0.0f && cData.depth > 0.0f && cData.depth > pDepth ) {
				cData.depth = pDepth;
				cData.wasCollision = true;
				cData.normal = ( pDepth == ( aMax - bMin ) ? -axes[axis] : axes[axis] );
			}
		}
		return cData;
	}

	CollisionData collisionTest( const ConvexHull& a, const Circle& b ) {
		CollisionData cData;
		cData.depth = INFINITY;
		std::vector<float> pDepths;

		for ( int vert = 0; vert < a.verts.size(); ++vert ) {
			const vec2 vecBetween = a.verts[vert] - a.verts[( vert + 1 ) % a.verts.size()];
			const vec2 axis = vecBetween.normal().perp();
			float aMin = INFINITY, aMax = -INFINITY;
			float bMin = INFINITY, bMax = -INFINITY;

			for ( int i = 0; i < a.verts.size(); ++i ) {
				float projection = dot( axis, a.verts[i] );
				aMin = fminf( projection, aMin );
				aMax = fmaxf( projection, aMax );
			}

			const float cirProj = dot( axis, b.pos );
			bMin = cirProj - b.radius;
			bMax = cirProj + b.radius;

			const float pDepth = fminf( aMax - bMin, bMax - aMin );

			// handle case where there was a gap and no collision
			if ( pDepth < 0.0f ) {
				cData.wasCollision = false;
				if ( pDepth > cData.depth || cData.depth >= 0.0f ) {
					cData.depth = pDepth;
				}
				cData.normal = ( pDepth == ( aMax - bMin ) ? axis : -axis );
			}
			// handle case where there is overlap and no gap has been detected yet
			else if ( pDepth >= 0.0f && cData.depth > 0.0f && cData.depth > pDepth ) {
				cData.depth = pDepth;
				cData.wasCollision = true;
				cData.normal = ( pDepth == ( aMax - bMin ) ? axis : -axis );
			}
		}
		return cData;
	}

	CollisionData collisionTest( const ConvexHull& a, const Ray& b ) {
		CollisionData cData = { false };
		cData.depth = INFINITY;
		std::vector<vec2> axes;

		// get all of the axes to project onto
		for ( int i = 0; i < a.verts.size(); ++i ) {
			const vec2 vecBetween = a.verts[i] - a.verts[( i + 1 ) % a.verts.size()];
			axes.push_back( vecBetween.normal().perp() );
		}

		float tMin = 0.0f, tMax = INFINITY;

		for ( int i = 0; i < axes.size(); ++i ) {
			// get the point plane distance
			// this is positive if ray starts in front of the edge
			const float N = dot( axes[i], b.pos - a.verts[i] );
			// get the relative direction of the ray compared to the edge normal
			// this is positive when the ray is facing the outside of the edge
			const float D = -dot( axes[i], b.dir );

			// if D is 0, then it is parallel with the line so it will never collide
			if ( fabs( D ) < FLT_EPSILON ) {
				continue;
			}
			// get the distance from the ray start to the plane
			const float dist = N / D;

			if ( D > 0.0f ) {
				// when entering a side
				// set min to the farthest entry point
				tMin = fmaxf( tMin, dist );
			} else {
				// when exiting side
				tMax = fminf( tMax, dist );
			}
		}
		cData.normal = -b.dir;
		cData.depth = ( tMin == 0.0f ? 0.0f : b.len - tMin );
		cData.wasCollision = ( tMin < tMax && tMin < b.len );
		return cData;
	}

	CollisionData collisionTest( const ConvexHull& a, const Plane& b ) {
		CollisionData cData;
		cData.normal = b.normal;
		cData.depth = INFINITY;

		// loop over vertices
		for ( int i = 0; i < a.verts.size(); ++i ) {
			// get points distance form the plane
			const float pp = dot( b.normal, a.verts[i] - b.pos );
			// keep track of min point
			cData.depth = min( pp, cData.depth );
		}

		// if less than 0 there was an overlap
		cData.wasCollision = cData.depth <= 0.0f;
		cData.depth = -cData.depth;
		return cData;
	}

	CollisionData collisionTest( const ConvexHull& a, const AABB& b ) {
		CollisionData cData = { false };
		cData.depth = INFINITY;
		std::vector<vec2> axes;
		const std::vector<vec2> aabbPoints = { b.min(), b.max(), {b.min().x, b.max().y}, {b.max().x, b.min().y} };

		for ( int i = 0; i < a.verts.size(); ++i ) {
			vec2 vecBetween = a.verts[i] - a.verts[( i + 1 ) % a.verts.size()];
			axes.push_back( vecBetween.normal().perp() );
		}
		axes.push_back( vec2{ 0.0f, 1.0f } );
		axes.push_back( vec2{ 1.0f, 0.0f } );

		for ( int axis = 0; axis < axes.size(); ++axis ) {
			float aMin = INFINITY, aMax = -INFINITY;
			float bMin = INFINITY, bMax = -INFINITY;

			for ( int i = 0; i < a.verts.size(); ++i ) {
				const float projection = dot( axes[axis], a.verts[i] );
				aMin = fminf( projection, aMin );
				aMax = fmaxf( projection, aMax );
			}
			for ( int i = 0; i < aabbPoints.size(); ++i ) {
				float projection = dot( axes[axis], aabbPoints[i] );
				bMin = fminf( projection, bMin );
				bMax = fmaxf( projection, bMax );
			}

			float pDepth = fminf( aMax - bMin, bMax - aMin );

			// handle case where there was a gap and no collision
			if ( pDepth < 0.0f ) {
				cData.wasCollision = false;
				if ( pDepth > cData.depth || cData.depth >= 0.0f ) {
					cData.depth = pDepth;
				}
				cData.normal = ( pDepth == ( aMax - bMin ) ? axes[axis] : -axes[axis] );
			}
			// handle case where there is overlap and no gap has been detected yet
			else if ( pDepth >= 0.0f && cData.depth > 0.0f && cData.depth > pDepth ) {
				cData.depth = pDepth;
				cData.wasCollision = true;
				cData.normal = ( pDepth == ( aMax - bMin ) ? axes[axis] : -axes[axis] );
			}
		}
		return cData;
	}

	CollisionData collisionTest( const AABB& a, const AABB& b ) {
		// get all of the different overlaps
		CollisionData cData;
		const float left = b.min().x - a.max().x;
		const float right = b.max().x - a.min().x;
		const float top = b.min().y - a.max().y;
		const float bottom = b.max().y - a.min().y;

		if ( left > 0.0f || right < 0.0f || top > 0.0f || bottom < 0.0f ) {
			// there is no overlap

			// get min horizontal movement needed for an x overlap
			const float gapX = -abs( left ) > right ? -abs( left ) : right;
			// get min vertical movement needed for a y overlap
			const float gapY = -abs( top ) > bottom ? -abs( top ) : bottom;

			if ( left > 0.0f || right < 0.0f ) {
				cData.normal.x = ( gapX == right ? gapX : -gapX );
			}
			if ( top > 0.0f || bottom < 0.0f ) {
				cData.normal.y = ( gapY == bottom ? gapY : -gapY );
			}
			cData.depth = -cData.normal.length();
			cData.normal = cData.normal.normal();
		} else {
			// there is an overlap
			const float collX = abs( left ) < right ? abs( left ) : right;
			const float collY = abs( top ) < bottom ? abs( top ) : bottom;
			if ( collX <= collY ) {
				cData.depth = collX;
				cData.normal = ( collX == right ? vec2{ 1.0f, 0.0f } : vec2{ -1.0f, 0.0f } );
			} else {
				cData.depth = collY;
				cData.normal = ( collY == bottom ? vec2{ 0.0f, 1.0f } : vec2{ 0.0f, -1.0f } );
			}
		}
		cData.wasCollision = cData.depth >= 0.0f;
		return cData;
	}

	CollisionData collisionTest( const Circle& a, const AABB& b ){
		CollisionData cData;
		vec2 point = { clamp( a.pos.x, b.min().x, b.max().x ), clamp( a.pos.y, b.min().y, b.max().y ) };
		const float dist = ( point - a.pos ).length();

		// get collision normal
		vec2 normal = { 0.0f, 0.0f };
		if ( point.x == b.min().x ) { cData.normal.x = -1.0f; } else if ( point.x == b.max().x ) { cData.normal.x = 1.0f; }
		if ( point.y == b.min().y ) { cData.normal.y = -1.0f; } else if ( point.y == b.max().y ) { cData.normal.y = 1.0f; }
		cData.depth = a.radius - dist;
		cData.wasCollision = ( cData.depth > 0.0f );

		return cData;
	}

	CollisionData collisionTest( const AABB& a, const Plane& b ) {
		CollisionData cData;
		const float distToPlane = dot( b.normal, a.pos - b.pos );
		const float projExtents = a.hExtents.x * abs( dot( b.normal, { 1.0f, 0.0f } ) ) + a.hExtents.y * abs( dot( b.normal, { 0.0f, 1.0f } ) );
		cData.depth = projExtents - distToPlane;
		cData.normal = b.normal;
		cData.wasCollision = cData.depth >= 0.0f;
		return cData;
	}

	CollisionData collisionTest( const AABB& a, const Ray& b ) {
		CollisionData cData;

		// return depth of 0 and collision if ray starts within aabb
		if ( b.pos.x > a.min().x && b.pos.x < a.max().x
			&& b.pos.y > a.min().y && b.pos.y < a.max().y ) {
			cData.depth = 0.0f;
			cData.wasCollision = true;
			return cData;
		}

		// Min/Max along the X-Axis
		const Plane s1p1 = Plane{ a.max(), {1.0f, 0.0f} };
		const Plane s1p2 = Plane{ a.min(),{ -1.0f, 0.0f } };
		const float t11 = RayPlaneDist( b, s1p1 ); // Right
		const float t12 = RayPlaneDist( b, s1p2 ); // Left
		const float t1min = std::fminf( t11, t12 );
		const float t1max = std::fmaxf( t11, t12 );
		const vec2  n1 = ( t11 < t12 ) ? vec2{ 1.0f, 0.0f } : vec2{ -1.0f, 0.0f };

		// Min/Max along the Y-Axis
		const Plane s2p1 = Plane{ a.max(), {0.0f, 1.0f} };
		const Plane s2p2 = Plane{ a.min(), {0.0f, -1.0f} };
		const float t21 = RayPlaneDist( b, s2p1 );
		const float t22 = RayPlaneDist( b, s2p2 );
		const float t2min = std::fminf( t21, t22 );
		const float t2max = std::fmaxf( t21, t22 );
		const vec2  n2 = ( t21 < t22 ) ? vec2{ 0.0f, 1.0f } : vec2{ 0.0f, -1.0f };

		const float tmin = std::fmaxf( t2min, t1min );
		const float tmax = std::fminf( t2max, t1max );

		cData.depth = b.len - tmin;
		cData.wasCollision = ( cData.depth >= 0 && tmin <= tmax && tmin > 0 );
		cData.normal = ( t1min > t2min ) ? n1 : n2;

		return cData;
	}

	CollisionData collisionTest( const Circle& a, const Circle& b ) {
		CollisionData cData;
		const float dist = ( b.pos - a.pos ).length();
		cData.depth = ( a.radius + b.radius ) - dist;
		cData.wasCollision = cData.depth > 0.0f;
		cData.normal = ( a.pos - b.pos ).normal();
		return cData;
	}

	CollisionData collisionTest( const Circle& a, const Plane& b ) {
		CollisionData cData;
		const float distToPlane = dot( b.normal, ( a.pos - b.pos ) );
		cData.depth = a.radius - distToPlane;
		cData.wasCollision = distToPlane <= a.radius || distToPlane < 0.0f;
		cData.normal = b.normal;
		return cData;
	}

	CollisionData collisionTest( const Circle& a, const Ray& b ) {
		CollisionData cData;
		const vec2 closestPoint = b.pos + b.dir * clamp( dot( ( a.pos - b.pos ), b.dir ), 0.0f, b.len );
		const vec2 distVec = closestPoint - b.pos;
		const float dist = ( a.pos - closestPoint ).length();
		cData.wasCollision = ( dist <= a.radius );
		cData.depth = a.radius - dist;
		cData.normal = -b.dir;
		return cData;
	}

	// Reference : http://www.codeproject.com/Tips/862988/Find-the-Intersection-Point-of-Two-Line-Segments
	CollisionData collisionTest( const Ray& a, const Ray& b ) {
		CollisionData cData;
		cData.normal = -a.dir;
		const vec2& p1 = a.pos;
		const vec2 p2 = a.pos + ( a.dir * a.len );
		const vec2& q1 = b.pos;
		const vec2 q2 = b.pos + ( b.dir * b.len );

		const vec2 r = p2 - p1;
		const vec2 s = q2 - q1;
		const float rCrossS = cross( r, s );
		const float qpCrossR = cross( q1 - p1, r );

		// if r x s = 0 and ( p - q ) x r = 0 then the two lines are collinear
		if ( abs( rCrossS ) < FLT_EPSILON && abs( qpCrossR ) < FLT_EPSILON ) {
			// 1. the two lines are collinear and overlapping
			if ( ( 0 <= dot( q1 - p1, r ) && dot( q1 - p1, r ) <= 1 ) || ( 0 <= dot( p1 - q1, s ) && dot( p1 - q1, s ) <= 1 ) ) {
				cData.wasCollision = true;
			}
			else {
				// 2. the lines are collinear but are disjoint
				cData.wasCollision = false;
			}
		}
		// 3. the two lines are parallel and non-intersecting
		else if ( abs( rCrossS ) < FLT_EPSILON && !abs( qpCrossR ) >= FLT_EPSILON ) {
			cData.wasCollision = false;
		}

		const float t = cross( q1 - p1, s ) / rCrossS;
		const float u = cross( q1 - p1, r ) / rCrossS;

		// 4. the two lines meet at the point p + tr = q + us
		if ( !abs(rCrossS) < FLT_EPSILON && (0.0f <= t && t <= 1.0f) && (0.0f <= u && u <= 1.0f) ) {
			cData.wasCollision = true;
			cData.depth = (1.0f - t) * a.len;
		}
		else {
			if( ( t > 1.0f ) && ( 0.0f <= u && u <= 1.0f ) ) cData.depth = ( 1.0f - t ) * a.len;
			cData.wasCollision = false;
		}
		return cData;
	}

	CollisionData collisionTest( const Ray& a, const Plane& b ) {
		// gets the shortest vector to move out of collision with Plane
		CollisionData cData{ true };
		const float dotVal = -dot( b.normal, a.dir );
		const float pointPos = dot( b.normal, vec2{ a.pos - b.pos } );
		// if not facing the plane
		if ( dotVal <= 0.0f ) {
			// colliding only if behind the plane
			cData.wasCollision = ( pointPos < 0.0f );
		} else {
			// get distance to plane
			const float rayPlaneDistance = pointPos / dotVal;
			cData.depth = a.len - rayPlaneDistance;
			cData.normal = -a.dir;
			cData.wasCollision = ( cData.depth > 0.0f );
		}
		return cData;
	}

	CollisionData collisionTest( const Plane& a, const Plane& b ) {
		// overlapping if a is behind b, b is behind a, or no facing eachother
		bool overlapping = ( abs( -1.0f - dot( a.normal, b.normal ) ) > FLT_EPSILON )
			|| ( dot( a.pos - b.pos, b.normal ) < 0.0f ) || ( dot( b.pos - a.pos, a.normal ) < 0.0f );
		return CollisionData{ overlapping };
	}
}