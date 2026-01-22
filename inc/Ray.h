#pragma once
#include "Color.h"
#include "Material.h"
#include "Utils.h"
#include "Vec.h"
#include <optional>

class Ray
{
public:
	explicit constexpr Ray(const Vec3& origin, const Vec3& direction)
		: m_origin{ origin }
		, m_direction{ direction }
	{
	}

	[[nodiscard]] constexpr auto get_origin() const noexcept -> const Vec3&
	{
		return m_origin;
	}

	[[nodiscard]] constexpr auto get_direction() const noexcept -> const Vec3&
	{
		return m_direction;
	}

	[[nodiscard]] constexpr auto at(const float t) const noexcept -> Vec3
	{
		return m_origin + (t * m_direction);
	}

	constexpr auto operator=(const Ray& other) -> Ray&
	{
		if (this != &other)
		{
			m_origin = other.m_origin;
			m_direction = other.m_direction;
		}
		return *this;
	}
	
	// Deffered member access pattern for now
	template<typename HitResult>
	[[nodiscard]] auto scatter(HitResult& hit_result) const -> std::optional<struct ScatterResult>;

private:
	Vec3 m_origin;
	Vec3 m_direction;
};

struct ScatterResult
{
	Ray ray{ Vec3{0.f,0.f,0.f}, Vec3{0.f,0.f,0.f} };
	Color3 attenuation{ 0.f, 0.f, 0.f };
};

template<typename HitResult>
[[nodiscard]] auto Ray::scatter(HitResult& hit_result) const -> std::optional<ScatterResult>
{
	ScatterResult result{};
	switch (hit_result.material.type)
	{
	case Material::Type::LAMBERTIAN:
	{
		result.attenuation = hit_result.material.albedo;
		Vec3 scatter_direction{ utility_functions::random_lambertian_distribution_vec(hit_result.normal) };

		if (utility_functions::near_zero_vec3(scatter_direction))
		{
			scatter_direction = hit_result.normal;
		}

		result.ray = Ray{ hit_result.hit_point, scatter_direction };
		break;
	}
	case Material::Type::METAL:
	{
		Vec3 scatter_direction{ utility_functions::reflect(m_direction, hit_result.normal) };
		// add fuzzines
		scatter_direction = scatter_direction.unit_vector() + (hit_result.material.fuzz * utility_functions::random_unit_vec());

		if (Vec3::dot(scatter_direction, hit_result.normal) < 0.0)
		{
			// ray absorbed, no scattering
			return {};
		}

		result.attenuation = hit_result.material.albedo;
		result.ray = Ray{ hit_result.hit_point, scatter_direction };
		break;
	}
	case Material::Type::DIELECTRIC:
	{
		result.attenuation = Color3{ 1.f, 1.f, 1.f };

		const double refraction_index{ hit_result.front_face ? (1.0 / hit_result.material.refraction_index) : hit_result.material.refraction_index };
		const double cos_theta{ std::fmin(-Vec3::dot(m_direction, hit_result.normal), 1.0) };
		const double sin_theta{ std::sqrt(1 - cos_theta * cos_theta) };

		if (
			(refraction_index * sin_theta > 1.0)
			||
			(utility_functions::schlick_approximation(cos_theta, refraction_index) > utility_functions::random_real_number())
			)
		{
			// reflection
			const Vec3 scatter_direction{ utility_functions::reflect(m_direction, hit_result.normal) };
			result.ray = Ray{ hit_result.hit_point, scatter_direction };
		}
		else
		{
			// refraction
			const Vec3 refraction_direction{ utility_functions::refract(m_direction.unit_vector(), hit_result.normal, refraction_index) };
			result.ray = Ray{ hit_result.hit_point, refraction_direction };
		}
		break;
	}
	}

	return std::optional<ScatterResult>{result};
}
