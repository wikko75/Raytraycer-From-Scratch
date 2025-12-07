#pragma once
#include <cstdint>
#include <Vec.h>

struct Viewport
{
	float width{};
	float height{ 2.f };

	Vec3 u{ 0.f,0.f,0.f };
	Vec3 v{ 0.f,0.f,0.f };

	Vec3 pixel_delta_u{ 0.f,0.f,0.f };
	Vec3 pixel_delta_v{ 0.f,0.f,0.f };

	explicit constexpr Viewport(uint32_t image_width, uint32_t image_height)
	{
		width = height * (static_cast<float>(image_width) / image_height);

		u = Vec3{ width, 0.f, 0.f };
		v = Vec3{ 0.f, -height, 0.f };

		pixel_delta_u = Vec3{ u / image_width };
		pixel_delta_v = Vec3{ v / image_height };
	}
};
