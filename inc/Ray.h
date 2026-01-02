#pragma once
#include "Color.h"
#include "Material.h"
#include "Utils.h"
#include "Vec.h"

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
	[[nodiscard]] auto scatter(HitResult& hit_result) const -> struct ScatterResult
	{
		ScatterResult result{};
		switch (hit_result.material->type)
		{
			case Material::Type::LAMBERTIAN:
			{
				result.attenuation = hit_result.material->albedo;
				result.ray = Ray{ hit_result.hit_point, utility_functions::random_lambertian_distribution_vec(hit_result.normal) };
				break;
			}
			case Material::Type::METAL:
			{
				// TODO: Metal scattering
				break;
			}
		}

		return result;
	}

private:
	Vec3 m_origin;
	Vec3 m_direction;
};

struct ScatterResult
{
	Ray ray{ Vec3{0.f,0.f,0.f}, Vec3{0.f,0.f,0.f} };
	Color3 attenuation{ 0.f, 0.f, 0.f };
};
