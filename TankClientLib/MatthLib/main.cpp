#include <iostream>
#include <cassert>
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat3.h"
#include "AABB.h"

//#define TEST_VEC2 false

int main() {

#ifdef TEST_VEC2
	matth::vec2 v1 = { 0, 1 };

	// test accest
	assert( (v1.x == 0.0f && v1.y == 1.0f) && "X Y ACCESS FAILED" );
	assert( ( v1[0] == 0.0f && v1[1] == 1.0f ) && "X Y ACCESS FAILED" );

	// test comparisons
	matth::vec2 v2 = v1;
	assert( v1 == v2 && "EQUALS FAILED" );

#endif

	std::vector<matth::vec2> points = { {4.0f, 5.0f},{ -5.4f, 2.3f },{ -7.0f, -1.0f },{ 5.0f, -2.0f } };
	matth::AABB aabb = matth::AABB::genAABB( points );
	std::cout << aabb.dim.x << " " << aabb.dim.y << std::endl;
	std::cout << aabb.pos.x << " " << aabb.pos.y << std::endl << std::endl;
	aabb.rotate( 135.0f );
	std::cout << aabb.dim.x << " " << aabb.dim.y << std::endl;
	std::cout << aabb.pos.x << " " << aabb.pos.y << std::endl << std::endl;

	system( "pause" );

	return 0;
}