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
	explicit constexpr Camera(const Vec3& position, const Viewport& viewport, float focal_length, uint32_t samples_count = 100)
		: m_position{ position }
		, m_viewport{ viewport }
		, m_focal_length{ focal_length }
		, m_samples_count{ samples_count }
	{
		m_viewport_upper_left = m_position - Vec3{ 0.f, 0.f, m_focal_length } - m_viewport.u / 2 - m_viewport.v / 2;
		m_pixel_00_location = m_viewport_upper_left + 0.5 * (m_viewport.pixel_delta_u + m_viewport.pixel_delta_v);
	}

	// Sends ray to specified image pixel - (row, column)
	// Return: Color3 for specified image pixel
	[[nodiscard]] constexpr auto send_ray(uint32_t row, uint32_t column, const HittableList& objects) const -> Color3
	{
		// perform antialiasing by sending m_samples_count rays
		// to pixels offset by random offset in [-.5,.5]
		// computing resulting sample value by blending rays together

		const Vec3 base_pixel{ m_pixel_00_location + (column * m_viewport.pixel_delta_u) + (row * m_viewport.pixel_delta_v) };

		Color3 result{ 0.f,0.f,0.f };
		for (uint32_t i{ 0 }; i < m_samples_count; ++i)
		{
			// random offset
			const float rx{ utility_functions::random_real_number(-0.5f, 0.5f) };
			const float ry{ utility_functions::random_real_number(-0.5f, 0.5f) };

			const Vec3 pixel_position{ base_pixel + (rx * m_viewport.pixel_delta_u) + (ry * m_viewport.pixel_delta_v) };
			const Vec3 ray_direction{ pixel_position - m_position };

			const Ray ray{ m_position, ray_direction };
			result += ray_color(ray, objects);
		}

		return result / m_samples_count;
	}

private:

	[[nodiscard]] constexpr auto ray_color(const Ray& ray, const HittableList& objects) const -> Color3
	{
		const auto hit_result{ objects.hit(ray, Interval{0.f, constants::infinity}) };

		if (hit_result.has_value())
		{
			// map normal to RGB [0,1] range
			return 0.5f * (hit_result.value().normal + Vec3{ 1.f, 1.f, 1.f });
		}

		const Vec3 rey_direction_normalized{ ray.get_direction().unit_vector() };
		return utility_functions::lerp(Color3{ 1.f, 1.f, 1.f }, Color3{ 0.f,0.f,1.f }, 0.5f * (rey_direction_normalized.y() + 1.f));
	}

private:
	Vec3 m_position;
	Viewport m_viewport;
	float m_focal_length;
	uint32_t m_samples_count;

	Vec3 m_viewport_upper_left{ 0.f,0.f,0.f };
	Vec3 m_pixel_00_location{ 0.f,0.f,0.f };
};
