#pragma once
#include "mat4.h"
#include "CollisionShapes.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Collider.h"

namespace matth {

	void drawMatrix( const mat3&, unsigned TINT );
	void drawVector( const mat3&, const vec2&, unsigned TINT );
	void drawPlane( const mat3&, const Plane&, unsigned TINT );
	void drawAABB( const mat3&, const AABB&, unsigned TINT );
	void drawCircle( const mat3&, const Circle&, unsigned TINT );
	void drawRay( const mat3&, const Ray&, unsigned TINT );

	void drawTransform( const Transform& );
	void drawRigidBody( const Transform&, const RigidBody& );
	void drawCollider( const Transform&, const Collider& );
}
