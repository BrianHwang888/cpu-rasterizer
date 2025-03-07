#pragma once

#include <cstdint>

namespace rasterizer {
	struct vector3f {
		float x, y, z;
	};

	struct vector4f {
		float x, y, z, w;
	};

	inline vector4f as_vector(const vector3f& v) {
		return {v.x, v.y, v.z, 0.0f};
	}

	inline vector4f as_point(const vector3f& v) {
		return {v.x, v.y, v.z, 1.0f};
	}

	inline vector4f operator - (const vector4f& v0, const vector4f& v1) {
		return {v0.x - v1.x, v0.y - v1.y, v0.z - v1.z, v0.w - v1.w};
	}

        inline vector4f operator + (const vector4f& v0, const vector4f& v1) {
		return {v0.x + v1.x, v0.y + v1.y, v0.z + v1.z, v0.w + v1.w};
	}

	inline vector4f operator * (const float f, const vector4f& v) {
		return {f * v.x, f * v.y, f * v.z, f * v.w};
	}

	inline float det2D(const vector4f& v0, const vector4f& v1) {
		return v0.x * v1.y - v0.y * v1.x;
	}

	inline vector4f perspective_divide(vector4f v) {
		v.x /= v.w;
		v.y /= v.w;
		v.z /= v.w;
		return v;
	}
}
