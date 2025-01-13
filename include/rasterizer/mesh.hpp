#pragma once

#include "rasterizer/attribute.hpp"
#include "rasterizer/types.hpp"

namespace rasterizer {
	struct mesh {
		attribute<vector3f> positions = {};
		std::uint32_t vertex_count = 0;
		attribute<vector4f> color = {};
	};
}
