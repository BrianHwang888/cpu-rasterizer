#pragma once

#include "rasterizer/types.hpp"

namespace rasterizer {
	struct mesh {
		vector3f const* positions = nullptr;
		std::uint32_t vertex_count = 0;
		vector4f color = {1.0f, 1.0f, 1.0f, 1.0f};
	};
}
