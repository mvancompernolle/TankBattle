#pragma once
#ifndef AABB_H
#define AABB_H

#include "vec2.h"
#include <vector>
#include <limits>

namespace matth {
	struct AABB {
		vec2 pos, dim;

		static AABB genAABB( const std::vector<vec2>& points) {
			AABB boundingBox;
			float minX = std::numeric_limits<float>::max(), minY = std::numeric_limits<float>::max();
			float maxX = std::numeric_limits<float>::min(), maxY = std::numeric_limits<float>::min();
			for ( vec2 point : points ) {
				if ( point.x < minX ) { minX = point.x; }
				if ( point.y < minY ) { minY = point.y; }
				if ( point.x > maxX ) { maxX = point.x; }
				if ( point.y > maxY ) { maxY = point.y; }
			}
			boundingBox.dim.x = maxX - minX;
			boundingBox.dim.y = maxY - minY;
			boundingBox.pos.x = minX + boundingBox.dim.x * 0.5f;
			boundingBox.pos.y = minY + boundingBox.dim.y * 0.5f;
			return boundingBox;
		}

		void rotate( float angle ) {
			float rads = angle * ( PI / 180.0f );
			float width = dim.x * std::abs(std::cos( rads )) + dim.y * std::abs(std::sin( rads ));
			float height = dim.y * std::abs(std::cos( rads )) + std::abs(dim.x * std::sin( rads ));
			dim.x = width;
			dim.y = height;
		}
	};
}

#endif // AABB_H
