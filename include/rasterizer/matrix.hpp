#pragma once

#include <cstdint>

#include "rasterizer/vector.hpp"

namespace rasterizer {
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

		static matrix4x4f scale(const vector3f& s) {
			return matrix4x4f {
				s.x, 0.0f, 0.0f, 0.0f,
				0.0f, s.y, 0.0f, 0.0f,
				0.0f, 0.0f, s.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f,
			};
		}

		static matrix4x4f scale(float s) {
			return scale(vector3f{s, s, s});
		}

		static matrix4x4f translate(const vector3f& s) {
			return matrix4x4f {
				1.0f, 0.0f, 0.0f, s.x,
				0.0f, 1.0f, 0.0f, s.y,
				0.0f, 0.0f, 1.0f, s.z,
				0.0f, 0.0f, 0.0f, 1.0f,
			};
		}

		static matrix4x4f rotateXY(float angle) {
			float cos = std::cos(angle);
			float sin = std::sin(angle);

			return matrix4x4f {
				cos, -sin, 0.0f, 0.0f,
				sin, cos, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f,
			};
		}

		static matrix4x4f rotateYZ(float angle) {
			float cos = std::cos(angle);
			float sin = std::sin(angle);

			return matrix4x4f {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, cos, -sin, 0.0f,
				0.0f, sin, cos, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f,
			};
		}
		
		static matrix4x4f rotateZX(float angle) {
			float cos = std::cos(angle);
			float sin = std::sin(angle);

			return matrix4x4f {
				cos, 0.0f, sin, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				-sin, 0.0f, cos, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f,
			};
		}

		static matrix4x4f perspective(float near, float far, float fovY, float aspect_ratio) {
			float top = near * std::tan(fovY / 2.f);
			float right = top * aspect_ratio;

			return matrix4x4f
			{
				near / right, 0.f, 0.f, 0.f,
				0.f, near / top, 0.f, 0.f,
				0.f, 0.f, -(far + near) / (far - near), - 2.f * far * near / (far - near),
				0.f, 0.f, -1.f, 0.f,
			};
		}
	};

	inline vector4f operator * (const matrix4x4f& m, const vector4f& v) {
		vector4f result {0.0f, 0.0f, 0.0f, 0.0f};

		result.x = m.values[0] * v.x + m.values[1] * v.y + m.values[2] * v.z + m.values[3] * v.w;
		result.y = m.values[4] * v.x + m.values[5] * v.y + m.values[6] * v.z + m.values[7] * v.w;
		result.z = m.values[8] * v.x + m.values[9] * v.y + m.values[10] * v.z + m.values[11] * v.w;
		result.w = m.values[12] * v.x + m.values[13] * v.y + m.values[14] * v.z + m.values[15] * v.w;

		return result;
	}
	
	inline matrix4x4f operator * (const matrix4x4f& m1, const matrix4x4f& m2) {
		matrix4x4f result {
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
		};

		for(int i = 0; i < 4; ++i) {
			for(int j = 0; j < 4; ++j) {
				for(int k = 0; k < 4; ++k) {
					result.values[4 * i + j] += m1.values[4 * i + k] * m2.values[4 * k + j];
				}
			}
		}

		return result;
	}

}
