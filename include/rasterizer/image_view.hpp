#pragma once

#include <cstdint>
#include "rasterizer/color.hpp"


namespace rasterizer {
	struct image_view {
		color4ub* pixels = nullptr;
		std::uint32_t width = 0;
		std::uint32_t height = 0;

		color4ub& at(std::uint32_t x, std::uint32_t y) const {
			return pixels[x + y * width];
		}
	};
}
