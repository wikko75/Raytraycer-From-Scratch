#include "Camera.h"
#include "Color.h"
#include "HittableList.h"
#include "Ppm.h"
#include "ProgressIndicator.h"
#include "Sphere.h"
#include "Vec.h"
#include "Viewport.h"
#include <cstdint>
#include "Material.h"
#include <memory>
#include <print>

namespace {


}

auto main() -> int
{
	constexpr float aspect_ratio{ 16.f / 9 };
	constexpr uint32_t image_width{ 400 };
	constexpr uint32_t image_height { (image_width / aspect_ratio) > 1 ? static_cast<uint32_t>(image_width / aspect_ratio) : 1 };

	std::print("AR: {}\nImage width: {}\nImage height: {}\n",
		aspect_ratio,
		image_width,
		image_height
	);

	constexpr PpmFile::Header header{.width=image_width, .height=image_height};

	PpmFile render{ "render.ppm", header };

	if (!render.is_open())
	{
		std::print("Can't open file to save!\n");
		return -1; 
	}

	constexpr Vec3 camera_position{ 5.f, 1.f, 1.f };

	constexpr Viewport viewport{ image_width, image_height };
	Camera camera{ Camera::Settings{
		.position = camera_position,
		.viewport = viewport,
		.look_at = Vec3{ 1.f, 0.f, -1.f },
		.fov = 20.f,
		.focal_length = 50.f,
		.f_stop = 8.f
	}};

	// Spheres
	HittableList objects{
		std::make_unique<Sphere>(
			Vec3{ 0.f, 0.f, -1.3f },
			.5f,
			Material{
				.type = Material::Type::LAMBERTIAN,
				.albedo = Color3{ 0.1f, 0.2f, 0.5f }
			}
		)
	};

	objects.add(
		std::make_unique<Sphere>(
			Vec3{ 0.f, -100.f, -1.f },
			99.5f,
			Material{
				.type = Material::Type::LAMBERTIAN,
				.albedo = Color3{ 0.8f, 0.8f, 0.0f }
			}
		)
	);
	
	objects.add(
		std::make_unique<Sphere>(
			Vec3{ -1.f, 0.f, -1.f },
			.5f,
			Material{
				.type = Material::Type::METAL,
				.albedo = Color3{ 0.8f, 0.3f, 0.0f },
				.fuzz = 0.2
			}
		)
	);

	objects.add(
		std::make_unique<Sphere>(
			Vec3{ 1.f, 0.f, -1.f },
			.5f,
			Material{
				.type = Material::Type::DIELECTRIC,
				.refraction_index = 1.5
			}
		)
	);
	objects.add(
		std::make_unique<Sphere>(
			Vec3{ 1.f, 0.f, -1.f },
			.4f,
			Material{
				.type = Material::Type::DIELECTRIC,
				.refraction_index = 1 / 1.5
			}
		)
	);

	ProgressIndicator progress_bar{ 100 };

	Color3 pixel_color{ 0.f,0.f,0.f };

	for (uint32_t row{ 0 }; row < header.height; ++row)
	{
		for (uint32_t column{ 0 }; column < header.width; ++column)
		{	
			pixel_color = camera.send_ray(row, column, objects);
			render.write_color(pixel_color);
		}
		
		progress_bar.update(static_cast<float>(row) / header.height);
		progress_bar.show();
	}
}