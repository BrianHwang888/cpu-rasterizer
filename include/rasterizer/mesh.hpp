#pragma once

#include "rasterizer/attribute.hpp"
#include "rasterizer/vector.hpp"

namespace rasterizer {
	struct mesh {
		attribute<vector3f> positions = {};
		attribute<vector4f> color = {};
		const std::uint32_t* indices = nullptr;
		std::uint32_t count = 0;
	};
}
