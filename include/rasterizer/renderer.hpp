#pragma once

#include "rasterizer/color.hpp"
#include "rasterizer/image_view.hpp"
#include "rasterizer/draw_command.hpp"
#include "rasterizer/viewport.hpp"

namespace rasterizer {
	void clear(const image_view& color_buffer, const vector4f& color);
	void draw(const image_view& color_buffer, const viewport& viewport, const draw_command& command);
}
