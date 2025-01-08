#pragma once

#include <algorithm>
#include <cstdint>

namespace rasterizer {
	struct color4ub {
		std::uint8_t r, g, b, a;
	};

	struct vector3f {
		float x, y, z;
	};

	struct vector4f {
		float x, y, z, w;
	};

	inline color4ub to_color4ub(vector4f const& c) {
		color4ub result;

		result.r = std::max(0.0f, std::min(255.f, c.x * 255.f));
		result.g = std::max(0.0f, std::min(255.f, c.y * 255.f));
		result.b = std::max(0.0f, std::min(255.f, c.z * 255.f));
		result.a = std::max(0.0f, std::min(255.f, c.w * 255.f));

		return result;
	}

	inline vector4f as_vector(const vector3f& v) {
		return {v.x, v.y, v.z, 0.0f};
	}

	inline vector4f as_point(const vector3f& v) {
		return {v.x, v.y, v.z, 1.0f};
	}

	inline vector4f operator - (const vector4f& v0, const vector4f& v1) {
		return {v0.x - v1.x, v0.y - v1.y, v0.z - v1.z, v0.w - v1.w};
	}

	inline float det2D(const vector4f& v0, const vector4f& v1) {
		return v0.x * v1.y - v0.y * v1.x;
	}	
}
