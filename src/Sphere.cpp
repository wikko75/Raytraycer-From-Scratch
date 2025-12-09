#include "Hittable.h"
#include "Interval.h"
#include "Ray.h"
#include "Sphere.h"
#include "Vec.h"
#include <cmath>
#include <optional>

Sphere::Sphere(const Vec3& position, float radius)
	: m_position{ position }
	, m_radius{ radius }
{
}

auto Sphere::hit(const Ray& ray, const Interval& ray_interval) const -> std::optional<HitResult>
{
	//const float a{ Vec3::dot(ray.get_direction(), ray.get_direction())};
	//const float b{ -2 * Vec3::dot(ray.get_direction(), sphere.position - ray.get_origin()) };
	//const float c{ Vec3::dot(sphere.position - ray.get_origin(), sphere.position - ray.get_origin()) - static_cast<float>(std::pow(sphere.redius, 2))};
	////sqrt not needed
	//const float delta{ std::pow(b, 2) - (4 * a * c) ) };

	//return delta >= 0 ? (- b - static_cast<float>( std::sqrt(delta) ) / (2 * a) : -1.f;

	//simplified
	const float a{ ray.get_direction().length_squared() };
	const float h{ Vec3::dot(ray.get_direction(), m_position - ray.get_origin()) };
	const float c{ (m_position - ray.get_origin()).length_squared() - m_radius * m_radius };

	const float delta{ (h * h) - (a * c) };

	if (delta < 0)
	{
		return {};
	}

	float t{ (h - std::sqrt(delta)) / a };

	if (!ray_interval.surrounds(t))
	{
		// try bigger root
		t = (h + std::sqrt(delta)) / a;

		if (!ray_interval.surrounds(t))
		{
			return {};
		}
	}

	const Vec3 hit_point{ ray.at(t) };
	const Vec3 normal{ (hit_point - m_position) / m_radius };
	
	HitResult hit_result{ .t = t, .hit_point = hit_point, .normal = normal, .front_face{} };
	hit_result.set_face_normal(ray, normal);

	return std::optional{ hit_result };
}
