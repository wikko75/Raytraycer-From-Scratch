#pragma once
#include "Interval.h"
#include "Utils.h"
#include "Vec.h"
#include "Viewport.h"
#include <Color.h>
#include <cstdint>
#include <HittableList.h>
#include <Ray.h>

class Camera
{
public:

	struct Settings
	{
		Vec3 position;
		Viewport viewport;
		Vec3 look_at;
		float fov = 90.f;
		uint32_t samples_count = 100;
		uint16_t ray_depth = 100;
	};

	explicit Camera(const Settings& settings)
		: m_settings{ settings }
		, m_focal_length{ (settings.position - settings.look_at).length()}
	{
		// cross product between (-direction and right) and (up and -direction) to setup proper direction vecs
		// why -direction? -> because right hand rule, we're going towards negative z axis
		m_direction = (settings.look_at - m_settings.position).unit_vector();
		m_right = Vec3::cross(m_up, -m_direction).unit_vector();
		m_up = Vec3::cross(-m_direction, m_right).unit_vector();

		m_settings.viewport.adjust(m_settings.fov, m_focal_length, m_up, m_right);
		m_viewport_upper_left = m_settings.position + m_direction * m_focal_length - m_settings.viewport.u / 2 - m_settings.viewport.v / 2;
		m_pixel_00_location = m_viewport_upper_left + 0.5 * (m_settings.viewport.pixel_delta_u + m_settings.viewport.pixel_delta_v);
	}


	// Sends ray to specified image pixel - (row, column)
	// Return: Color3 for specified image pixel
	[[nodiscard]] constexpr auto send_ray(uint32_t row, uint32_t column, const HittableList& objects) const -> Color3
	{
		// perform antialiasing by sending m_samples_count rays
		// to pixels offset by random offset in [-.5,.5]
		// computing resulting sample value by blending rays together

		const Vec3 base_pixel{ m_pixel_00_location + (column * m_settings.viewport.pixel_delta_u) + (row * m_settings.viewport.pixel_delta_v) };

		Color3 result{ 0.f,0.f,0.f };
		for (uint32_t i{ 0 }; i < m_settings.samples_count; ++i)
		{
			// random offset
			const float rx{ utility_functions::random_real_number(-0.5f, 0.5f) };
			const float ry{ utility_functions::random_real_number(-0.5f, 0.5f) };

			const Vec3 pixel_position{ base_pixel + (rx * m_settings.viewport.pixel_delta_u) + (ry * m_settings.viewport.pixel_delta_v) };
			const Vec3 ray_direction{ pixel_position - m_settings.position };

			const Ray ray{ m_settings.position, ray_direction };
			result += ray_color(ray, objects, 0);
		}

		return result / m_settings.samples_count;
	}

private:

	[[nodiscard]] constexpr auto ray_color(const Ray& ray, const HittableList& objects, uint32_t ray_depth) const -> Color3
	{
		if (ray_depth == m_settings.ray_depth)
		{
			return Color3{ 0.f,0.f,0.f };
		}

		const auto hit_result{ objects.hit(ray, Interval{0.001f, constants::infinity}) };

		if (!hit_result.has_value())
		{
			const Vec3 ray_direction_normalized{ ray.get_direction().unit_vector() };
			return utility_functions::lerp(Color3{ 1.f, 1.f, 1.f }, Color3{ 0.6f,0.6f,1.f }, 0.5f * (ray_direction_normalized.y() + 1.f));
		}

		const auto scatter_result{ ray.scatter(hit_result.value()) };

		if (!scatter_result.has_value())
		{
			return Color3{ 0.f,0.f,0.f };
		}

		return scatter_result->attenuation * ray_color(scatter_result->ray, objects, ++ray_depth);
	}

private:
	Settings m_settings;
	float m_focal_length;

	Vec3 m_viewport_upper_left{ 0.f,0.f,0.f };
	Vec3 m_pixel_00_location{ 0.f,0.f,0.f };
	
	Vec3 m_up{ 0.f, 1.f, 0.f };
	Vec3 m_right{ 1.f, 0.f, 0.f };
	Vec3 m_direction{ 0.f, 0.0f, -.1f };
};
