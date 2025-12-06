#include "Color.h"
#include "HittableList.h"
#include "Interval.h"
#include "Ppm.h"
#include "ProgressIndicator.h"
#include "Ray.h"
#include "Sphere.h"
#include "Utils.h"
#include "Vec.h"
#include <cstdint>
#include <memory>
#include <print>

namespace {

	auto lerp(const Vec3& first, const Vec3& second, float value) -> Vec3
	{
		return (1 - value) * first + value * second;
	}

	auto ray_color(const Ray& ray, const HittableList& objects)
	{
		const auto hit_result{ objects.hit(ray, Interval{0.f, constants::infinity}) };

		if (hit_result.has_value())
		{
			// map normal to RGB [0,1] range
			return 0.5f * (hit_result.value().normal + Vec3{ 1.f, 1.f, 1.f });
		}

		const Vec3 rey_direction_normalized{ ray.get_direction().unit_vector() };
		return lerp(Color3{ 1.f, 1.f, 1.f }, Color3{ 0.f,0.f,1.f }, 0.5f * (rey_direction_normalized.y() + 1.f));
	}
}

auto main() -> int
{
	constexpr float aspect_ratio{ 16.f / 9 };
	constexpr uint32_t image_width{ 400 };
	constexpr uint32_t image_height { (image_width / aspect_ratio) > 1 ? static_cast<uint32_t>(image_width / aspect_ratio) : 1 };

	constexpr float viewport_height{ 2.f };
	constexpr float viewport_width{ viewport_height * (static_cast<float>(image_width) / image_height) };

	std::print("AR: {}\nImage width: {}\nImage height: {}\nViewport width: {}\nViewport height: {}\nVAR: {}\n",
		aspect_ratio,
		image_width,
		image_height,
		viewport_width,
		viewport_height,
		viewport_width/viewport_height);

	constexpr PpmFile::Header header{.width=image_width, .height=image_height};

	constexpr Vec3 camera_position{ 0.f, 0.f, 0.f };
	constexpr float camera_focal_length{ 1.f };

	constexpr Vec3 viewport_u{ viewport_width, 0.f, 0.f };
	constexpr Vec3 viewport_v{ 0.f, -viewport_height, 0.f };

	constexpr Vec3 pixel_delta_u{ viewport_u / image_width };
	constexpr Vec3 pixel_delta_v{ viewport_v / image_height };

	constexpr Vec3 viewport_upper_left{ camera_position - Vec3{0.f, 0.f, camera_focal_length} - viewport_u / 2 - viewport_v / 2 };

	constexpr Vec3 pixel_00_location{ viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v) };

	std::print("Camera:\nposition: {}\nfocal length: {}\n\nViewport:\nu: {}\nv: {}\nupper_left: {}\n\npixel_delta_u: {}\npixel_delta_v: {}\npixel_00: {}\n",
		camera_position, camera_focal_length, viewport_u, viewport_v, viewport_upper_left, pixel_delta_u, pixel_delta_v, pixel_00_location);


	// Spheres
	auto sphere{ std::make_unique<Sphere>(Sphere{Vec3{ 0.f, 0.f, -1.f }, .3f }) };
	HittableList objects{ std::make_unique<Sphere>(Sphere{Vec3{ 0.f, 0.f, -1.f }, .3f }) };
	objects.add(std::make_unique<Sphere>(Sphere{ Vec3{ 0.f, -100.f, -1.f }, 99.5f }));
	
	PpmFile render{ "render.ppm", header };

	if (!render.is_open())
	{
		std::print("Can't open file to save!\n");
		return -1; 
	}

	ProgressIndicator progress_bar{ 100 };

	Vec3 pixel_position{ 0.f, 0.f, 0.f };
	Color3 pixel_color{ 0.f,0.f,0.f };

	for (uint32_t row{ 0 }; row < header.height; ++row)
	{
		for (uint32_t column{ 0 }; column < header.width; ++column)
		{
			pixel_position = pixel_00_location + (column * pixel_delta_u) + (row * pixel_delta_v);
			
			auto ray_direction{ pixel_position - camera_position };
			const Ray ray{ camera_position, ray_direction };
			
			pixel_color = ray_color(ray, objects);

			render.write_color(pixel_color);
		}
		
		progress_bar.update(static_cast<float>(row) / header.height);
		progress_bar.show();
	}
}