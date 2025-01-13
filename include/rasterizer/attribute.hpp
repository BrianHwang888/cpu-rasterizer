#pragma once

#include <cstdint>

namespace rasterizer {
	template <typename T> struct attribute {
		const void* ptr = nullptr;
		std::uint32_t stride = sizeof(T);
		
		//Access attribute data
		const T& operator[] (std::uint32_t i) const {
			return *(const T*)((const char*)(ptr) + stride * i);
		}
	};
}
