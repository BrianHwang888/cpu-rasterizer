#pragma once

#include "rasterizer/types.hpp"
#include "rasterizer/image_view.hpp"
#include "rasterizer/draw_command.hpp"

namespace rasterizer {
	void clear(const image_view& color_buffer, const vector4f& color);
	void draw(const image_view& color_buffer, const draw_command& command);
}
