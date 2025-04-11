#include <algorithm>
#include <cmath>

#include "rasterizer/renderer.hpp"

namespace rasterizer {

	namespace {
		struct vertex {
			vector4f position;
			vector4f color;
		};
	}

	//clears the screen with given color
	void clear(const image_view& color_buffer, const vector4f& color) {
		auto ptr = color_buffer.pixels;
		auto size = color_buffer.width * color_buffer.height;
		std::fill_n(ptr, size, to_color4ub(color));
	}


	vertex clip_intersect_edge(vertex const & v0, vertex const & v1, float value0, float value1) {
		float t = value0 / (value0 - value1);

		vertex v;
    		v.position = (1.0f - t) * v0.position + t * v1.position;
	    	v.color = (1.0f - t) * v0.color + t * v1.color;

	    	return v;
	}

	vertex* clip_triangle(vertex* triangle, vector4f equation, vertex* result) {
		float values[3] = {
			dot(triangle[0].position, equation),
			dot(triangle[1].position, equation),
			dot(triangle[2].position, equation),
		};

		std::uint8_t mask = (values[0] < 0.0f ? 1 : 0)
			| (values[1] < 0.0f ? 2 : 0)
			| (values[2] < 0.0f ? 4 : 0);

		switch (mask) {
			case 0b000:
				// All vertices are inside allowed half-space
				// No clipping required, copy the triangle to output
				*result++ = triangle[0];
				*result++ = triangle[1];
				*result++ = triangle[2];
				break;
			case 0b001:
				// Vertex 0 is outside allowed half-space
				// Replace it with points on edges 01 and 02
				// And re-triangulate
				{
					auto v01 = clip_intersect_edge(triangle[0], triangle[1], values[0], values[1]);
					auto v02 = clip_intersect_edge(triangle[0], triangle[2], values[0], values[2]);
					*result++ = v01;
					*result++ = triangle[1];
					*result++ = triangle[2];
					*result++ = v01;
					*result++ = triangle[2];
					*result++ = v02;
				}
				break;
			case 0b010:
				// Vertex 1 is outside allowed half-space
				// Replace it with points on edges 10 and 12
				// And re-triangulate
				{
					auto v10 = clip_intersect_edge(triangle[1], triangle[0], values[1], values[0]);
					auto v12 = clip_intersect_edge(triangle[1], triangle[2], values[1], values[2]);
					*result++ = triangle[0];
					*result++ = v10;
					*result++ = triangle[2];
					*result++ = triangle[2];
					*result++ = v10;
					*result++ = v12;
				}
				break;
			case 0b011:
				// Vertices 0 and 1 are outside allowed half-space
				// Replace them with points on edges 02 and 12
				*result++ = clip_intersect_edge(triangle[0], triangle[2], values[0], values[2]);
				*result++ = clip_intersect_edge(triangle[1], triangle[2], values[1], values[2]);
				*result++ = triangle[2];
				break;
			case 0b100:
				// Vertex 2 is outside allowed half-space
				// Replace it with points on edges 20 and 21
				// And re-triangulate
				{
					auto v20 = clip_intersect_edge(triangle[2], triangle[0], values[2], values[0]);
					auto v21 = clip_intersect_edge(triangle[2], triangle[1], values[2], values[1]);
					*result++ = triangle[0];
					*result++ = triangle[1];
					*result++ = v20;
					*result++ = v20;
					*result++ = triangle[1];
					*result++ = v21;
				}
				break;
			case 0b101:
				// Vertices 0 and 2 are outside allowed half-space
				// Replace them with points on edges 01 and 21
				*result++ = clip_intersect_edge(triangle[0], triangle[1], values[0], values[1]);
				*result++ = triangle[1];
				*result++ = clip_intersect_edge(triangle[2], triangle[1], values[2], values[1]);
				break;
			case 0b110:
				*result++ = triangle[0];
				*result++ = clip_intersect_edge(triangle[1], triangle[0], values[1], values[0]);
				*result++ = clip_intersect_edge(triangle[2], triangle[0], values[2], values[0]);
				break;
			case 0b111:
				// All vertices are outside allowed half-space
				// Clip the whole triangle, result is empty
				break;
		}

		return result;
	}

	vertex* clip_triangle(vertex* begin, vertex* end) {
		static const vector4f equations[2] = {
			{0.0f, 0.0f, 1.0f, 1.0f}, // Z + W > 0
			{0.0f, 0.0f, -1.0f, 1.0f}, // -Z + W > 0
		};

		vertex result[12];

		for(auto equation : equations) {
			auto result_end = result;
			for(vertex * triangle = begin; triangle != end; triangle += 3)
				result_end = clip_triangle(triangle, equation, result_end);
		end = std::copy(result, result_end, begin);
		}

		return end;
	}

	//fills color buffer with given color in given draw_command
	void draw(const image_view& color_buffer, const viewport& viewport, const draw_command& command) {
		for(std::uint32_t vertex_index = 0; vertex_index + 2 < command.mesh.count; vertex_index += 3) {
			std::uint32_t i0 = vertex_index;
			std::uint32_t i1 = vertex_index + 1;
			std::uint32_t i2 = vertex_index + 2;

			if(command.mesh.indices) {
				i0 = command.mesh.indices[i0];
				i1 = command.mesh.indices[i1];
				i2 = command.mesh.indices[i2];
			}

			vertex clipped_vertices[12];

			clipped_vertices[0].position = command.transform * as_point(command.mesh.positions[i0]);
			clipped_vertices[1].position = command.transform * as_point(command.mesh.positions[i1]);
			clipped_vertices[2].position = command.transform * as_point(command.mesh.positions[i2]);

			clipped_vertices[0].color = command.mesh.colors[i0];
			clipped_vertices[1].color = command.mesh.colors[i1];
			clipped_vertices[2].color = command.mesh.colors[i2];

			auto clipped_vertices_end = clip_triangle(clipped_vertices, clipped_vertices + 3);
			for(auto triangle_begin = clipped_vertices; triangle_begin != clipped_vertices_end; triangle_begin += 3) {
				auto v0 = triangle_begin[0];
				auto v1 = triangle_begin[1];
				auto v2 = triangle_begin[2];

				v0.position = perspective_divide(v0.position);
				v1.position = perspective_divide(v1.position);
				v2.position = perspective_divide(v2.position);

				v0.position = apply(viewport, v0.position);
				v1.position = apply(viewport, v1.position);
				v2.position = apply(viewport, v2.position);

				float det012 = det2D(v1.position - v0.position, v2.position - v0.position);
			
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


				std::int32_t x_min = std::max<std::int32_t>(viewport.x_min, 0);
				std::int32_t x_max = std::min<std::int32_t>(viewport.x_max, color_buffer.width) - 1;
				std::int32_t y_min = std::max<std::int32_t>(viewport.y_min, 0);
				std::int32_t y_max = std::min<std::int32_t>(viewport.y_max, color_buffer.height) - 1;

				x_min = std::max<float>(std::min({std::floor(v0.position.x), std::floor(v1.position.x), std::floor(v2.position.x)}), x_min);
				x_max = std::min<float>(std::max({std::floor(v0.position.x), std::floor(v1.position.x), std::floor(v2.position.x)}), x_max);
				y_min = std::max<float>(std::min({std::floor(v0.position.y), std::floor(v1.position.y), std::floor(v2.position.y)}), y_min);
				y_max = std::min<float>(std::max({std::floor(v0.position.y), std::floor(v1.position.y), std::floor(v2.position.y)}), y_max);

				for(std::int32_t y = y_min; y <= y_max; ++y) {
					for(std::int32_t x = x_min; x <= x_max; ++x) {
						vector4f p{x + 0.5f, y + 0.5f, 0.0f, 0.0f};

						float det01p = det2D(v1.position - v0.position, p - v0.position);
						float det12p = det2D(v2.position - v1.position, p - v1.position);
						float det20p = det2D(v0.position - v2.position, p - v2.position);

						if(det01p >= 0.0f && det12p >= 0.0f && det20p >= 0.0f) {

							//interpolate color using Barycentric coordinates
							float l0 = det12p / det012;
							float l1 = det20p / det012;
							float l2 = det01p / det012;


							//color_buffer.at(x, y) = to_color4ub(l0 * v0.color + l1 * v1.color + l2 * v2.color);
							auto color = l0 * v0.color + l1 * v1.color + l2 * v2.color;
							if(int(std::floor(color.x * 8) + std::floor(color.y * 8)) % 2 == 0)
								color_buffer.at(x, y) = {0, 0, 0, 255};
							else
								color_buffer.at(x, y) = {255, 255, 255, 255};
						}
					}
				}
			}
		}
	}
}
