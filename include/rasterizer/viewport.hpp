#pragma once

#include <cstdint>

#include "rasterizer/vector.hpp"

namespace rasterizer {

	struct viewport {
		std::int32_t x_min, y_min, x_max, y_max;
	};

	inline vector4f apply(const viewport& viewport, vector4f v) {
		v.x = viewport.x_min + (viewport.x_max - viewport.x_min) * (0.5f + 0.5f * v.x);
		v.y = viewport.y_min + (viewport.y_max - viewport.y_min) * (0.5f - 0.5f * v.y);
		return v;
	}
}
