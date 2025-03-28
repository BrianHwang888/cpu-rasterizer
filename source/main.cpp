#include <chrono>
#include <iostream>
#include <SDL2/SDL.h>

#include "rasterizer/cube.hpp"
#include "rasterizer/renderer.hpp"
#include "rasterizer/viewport.hpp"

int main() {
	SDL_Init(SDL_INIT_VIDEO);

	//window setup
	int width = 800;
	int height = 600;

	SDL_Surface* draw_surface = nullptr;
	SDL_Window* window = SDL_CreateWindow("Tiny Rasterizer",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			width, height,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

	int mouse_x = 0;
	int mouse_y = 0;
	
	auto last_frame_start = std::chrono::high_resolution_clock::now();

	float time = 0.0f;

	//draw loop
	bool running = true;
	while(running) {
		
		//capture window event
		for(SDL_Event event; SDL_PollEvent(&event);) switch (event.type) {
		case SDL_WINDOWEVENT:
			switch (event.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					width = event.window.data1;
					height = event.window.data2;
					break;
			}
			break;
		case SDL_QUIT:
			running = false;
			break;
		case SDL_MOUSEMOTION:
			mouse_x = event.motion.x;
			mouse_y = event.motion.y;
			break;
		case SDL_WINDOWEVENT_RESIZED:
			if(draw_surface)
				SDL_FreeSurface(draw_surface);
			draw_surface = nullptr;
			width = event.window.data1;
			height = event.window.data2;
			break;
		}

		if(!running)
			break;

		if(!draw_surface) {
			draw_surface = SDL_CreateRGBSurfaceWithFormat(0, width, height,
				32, SDL_PIXELFORMAT_RGBA32);
			SDL_SetSurfaceBlendMode(draw_surface, SDL_BLENDMODE_NONE);
		}

		auto now = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration_cast<std::chrono::duration<float>>
			(now - last_frame_start).count();
		last_frame_start = now;

		std::cout << dt << std::endl;

		time += dt;

		//setur color buffer
		rasterizer::image_view color_buffer {
			.pixels = (rasterizer::color4ub*)draw_surface->pixels,
			.width = (std::uint32_t)width,
			.height = (std::uint32_t)height,
		};

		rasterizer::viewport viewport {
			.x_min = 0,
			.y_min = 0,
			.x_max = (std::int32_t)color_buffer.width,
			.y_max = (std::int32_t)color_buffer.height,
		}; 

		rasterizer::clear(color_buffer, {0.8f, 0.9f, 1.0f, 1.0f});

		rasterizer::vector3f positions[] {
			{-0.5f, -0.5f, 0.0f},
			{-0.5f, 0.5f, 0.0f},
			{0.5f, -0.5f, 0.0f},
			{0.5f, 0.5f, 0.0f},
		};
		
		rasterizer::vector4f colors[] {
			{1.0f, 0.0f, 0.0f, 1.0f},
			{0.0f, 1.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f, 1.0f},
			{1.0f, 1.0f, 1.0f, 1.0f},
		};

		std::uint32_t indices[] {
			0, 1, 2,
			2, 1, 3,
		};

		
		rasterizer::matrix4x4f model = rasterizer::matrix4x4f::translate({0.f, 0.f, -4.f})
		    * rasterizer::matrix4x4f::rotateZX(time)
		    * rasterizer::matrix4x4f::rotateXY(time * 1.61f);

		rasterizer::matrix4x4f projection = rasterizer::matrix4x4f::perspective(4.f, 10.f, M_PI / 3.f, width * 1.f / height);

		draw(color_buffer, viewport,
		    rasterizer::draw_command{
		        .mesh = rasterizer::cube,
		        .cull_mode = rasterizer::cull_mode::ccw,
		        .transform = projection * model,
		    }
		);

		SDL_Rect rect {.x = 0, .y = 0, .w = width, .h = height};
		SDL_BlitSurface(draw_surface, &rect, SDL_GetWindowSurface(window), &rect);

		SDL_UpdateWindowSurface(window);
	}
}
