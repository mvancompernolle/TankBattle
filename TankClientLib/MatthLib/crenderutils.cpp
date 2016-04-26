#include "crenderutils.h"
#include "sfwdraw.h"

namespace matth {

	void drawMatrix( const mat3& mat, unsigned TINT ) {
		vec2 pos = mat[2].xy;
		vec2 up = pos + mat[1].xy;
		vec2 right = pos + mat[0].xy;

		sfw::drawLine( pos.x, pos.y, right.x, right.y, RED );
		sfw::drawLine( pos.x, pos.y, up.x, up.y, GREEN );
	}

	void drawVector( const mat3& mat, const vec2& v, unsigned TINT ) {
		vec2 a = (mat * vec3{v.x, v.y, 1.0f}).xy;
		sfw::drawLine( mat[2].x, mat[2].y, a.x, a.y, TINT );
	}

	void drawPlane( const mat3& mat, const Plane& plane, unsigned TINT ) {

	}

	void drawAABB( const mat3& mat, const AABB& aabb, unsigned TINT ) {

	}

	void drawCircle( const mat3& mat, const Circle& circle, unsigned TINT ) {

	}

	void drawRay( const mat3& mat, const Ray& ray, unsigned TINT ) {

	}

	void drawTransform( const Transform& transform ) {
		drawMatrix( transform.getGlobalTransform(), 0 );
	}

	void drawRigidBody( const Transform& transform, const RigidBody& rbody) {
		drawVector( transform.getGlobalTransform(), rbody.acc, 0 );
		drawVector( transform.getGlobalTransform(), rbody.vel, 0 );
	}

	void drawCollider( const Transform& transform, const Collider& collider) {
	}

}