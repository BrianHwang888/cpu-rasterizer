#include <algorithm>
#include <cmath>
#include "rasterizer/renderer.hpp"

namespace rasterizer {

	//clears the screen with given color
	void clear(const image_view& color_buffer, const vector4f& color) {
		auto ptr = color_buffer.pixels;
		auto size = color_buffer.width * color_buffer.height;
		std::fill_n(ptr, size, to_color4ub(color));
	}

	//fills color buffer with given color in given draw_command
	void draw(const image_view& color_buffer, const draw_command& command) {
		for(std::uint32_t vertex_index = 0; vertex_index + 2 < command.mesh.vertex_count; vertex_index += 3) {
			auto v0 = command.transform * as_point(command.mesh.positions[vertex_index]);
			auto v1 = command.transform * as_point(command.mesh.positions[vertex_index + 1]);
			auto v2 = command.transform * as_point(command.mesh.positions[vertex_index + 2]);

			auto c0 = command.mesh.color[vertex_index + 0];
			auto c1 = command.mesh.color[vertex_index + 1];
			auto c2 = command.mesh.color[vertex_index + 2];

			float det012 = det2D(v1 - v0, v2 - v0);
			
			//check if counter clockwise on screen
			bool const ccw = det012 < 0.0f;

			//cull overlapping triangles using back-face culling
			switch (command.cull_mode) {
				case cull_mode::none:
					break;
				case cull_mode::cw:
					if(!ccw)
						continue;
					break;
				case cull_mode::ccw:
					if(ccw)
						continue;
					break;
			}

			if(ccw) {
				std::swap(v1, v2);
				det012 = -det012;
			}

			std::int32_t x_min = std::min({std::floor(v0.x), std::floor(v1.x), std::floor(v2.x)});
			std::int32_t x_max = std::max({std::floor(v0.x), std::floor(v1.x), std::floor(v2.x)});
			std::int32_t y_min = std::min({std::floor(v0.y), std::floor(v1.y), std::floor(v2.y)});
			std::int32_t y_max = std::max({std::floor(v0.y), std::floor(v1.y), std::floor(v2.y)});

			x_min = std::max<std::int32_t>(0, x_min);
			x_max = std::min<std::int32_t>(color_buffer.width - 1, x_max);
			y_min = std::max<std::int32_t>(0, y_min);
			y_max = std::min<std::int32_t>(color_buffer.height - 1, y_max);

			for(std::int32_t y = y_min; y <= y_max; ++y) {
				for(std::int32_t x = x_min; x <= x_max; ++x) {
					vector4f p{x + 0.5f, y + 0.5f, 0.0f, 0.0f};

					float det01p = det2D(v1 - v0, p - v0);
					float det12p = det2D(v2 - v1, p - v1);
					float det20p = det2D(v0 - v2, p - v2);

					if(det01p >= 0.0f && det12p >= 0.0f && det20p >= 0.0f) {

						//interpolate color using Barycentric coordinates
						float l0 = det12p / det012;
						float l1 = det20p / det012;
						float l2 = det01p / det012;

						color_buffer.at(x, y) = to_color4ub(l0 * c0 + l1 * c1 + l2 * c2);
					}
				}
			}
		}
	}
}
