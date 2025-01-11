#pragma once

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

	struct matrix4x4f {
		float values[16];

		static matrix4x4f identity() {
			return matrix4x4f {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f,
			};
		}
	};

	template <typename T> T max(T a, T b) {
		return (a > b) ? a : b;
	}

	template <typename T> T min(T a, T b) {
		return (a < b) ? a : b;
	}

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

	inline vector4f operator * (const matrix4x4f& m, const vector4f& v) {
		vector4f result {0.0f, 0.0f, 0.0f, 0.0f};

		result.x = m.values[0] * v.x + m.values[1] * v.y + m.values[2] * v.z + m.values[3] * v.w;
		result.y = m.values[4] * v.x + m.values[5] * v.y + m.values[6] * v.z + m.values[7] * v.w;
		result.z = m.values[8] * v.x + m.values[9] * v.y + m.values[10] * v.z + m.values[11] * v.w;
		result.w = m.values[12] * v.x + m.values[13] * v.y + m.values[14] * v.z + m.values[15] * v.w;

		return result;
	}

	inline float det2D(const vector4f& v0, const vector4f& v1) {
		return v0.x * v1.y - v0.y * v1.x;
	}

}
