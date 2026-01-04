#pragma once
#include "Vec.h"
#include <limits>
#include <numbers>
#include <random>
#include <cmath>

namespace constants {

	constexpr double pi{ std::numbers::pi };
	constexpr double infinity{ std::numeric_limits<double>::infinity() };

}

namespace utility_functions {

	[[nodiscard]] constexpr auto radians(double degrees) -> double
	{
		return (degrees / 180) * constants::pi;
	}

	[[nodiscard]] constexpr auto degrees(double radians) -> double
	{
		return (radians / constants::pi) * 180;
	}

	[[nodiscard]] constexpr auto lerp(const Vec3& first, const Vec3& second, float value) -> Vec3
	{
		return (1 - value) * first + (value * second);
	}

	[[nodiscard]] inline auto random_real_number() -> float
	{
		thread_local std::mt19937 generator{ std::random_device{}() };
		thread_local std::uniform_real_distribution<float> dist{ 0.0f, 1.0f };
		return dist(generator);
	}

	[[nodiscard]] inline auto random_real_number(float range_min, float range_max) -> float
	{
		return range_min + (range_max - range_min) * random_real_number();
	}

	[[nodiscard]] inline auto random_vec3() -> Vec3
	{
		return Vec3{ random_real_number(), random_real_number(), random_real_number() };
	}

	[[nodiscard]] inline auto random_vec3(float range_min, float range_max) -> Vec3
	{
		return Vec3{ random_real_number(range_min, range_max), random_real_number(range_min, range_max), random_real_number(range_min, range_max) };
	}

	inline auto random_unit_vec() -> Vec3
	{
		Vec3 random_vec{ random_vec3(-1.f, 1.f) };
		double length_squared{ random_vec.length_squared() };

		while (length_squared > 1.f || length_squared < 1e-160)
		{
			random_vec = random_vec3(-1.f, 1.f);
			length_squared = random_vec.length_squared();
		}

		return random_vec.unit_vector();
	}

	inline auto random_on_sphere_vec(const Vec3& normal) -> Vec3
	{
		const Vec3 random_vec{ random_unit_vec() };

		if (Vec3::dot(random_vec, normal) > 0)
		{
			return random_vec;
		}

		return -random_vec;
	}

	inline auto random_lambertian_distribution_vec(const Vec3& normal) -> Vec3
	{
		return random_unit_vec() + normal;
	}

	[[nodiscard]] inline auto near_zero_vec3(const Vec3& vec) -> bool
	{
		constexpr double s{ 1e-8 };
		return (std::fabs(vec.x()) < s) && (std::fabs(vec.y()) < s) && (std::fabs(vec.z()) < s);
	}

	[[nodiscard]] inline auto reflect(const Vec3& vec, const Vec3& normal) -> Vec3
	{
		return vec - 2 * Vec3::dot(vec, normal) * normal;
	}

	/// \brief Calculates the refracted ray direction using Snell's law.
	/// \param vec The incident ray direction (must be normalized).
	/// \param normal The surface normal vector (must be normalized).
	/// \param refractive_index The ratio of refractive indices (n1/n2).
	/// \returns The refracted ray direction.
	[[nodiscard]] constexpr auto refract(const Vec3& vec, const Vec3& normal, float refractive_index) -> Vec3
	{
		const Vec3 refracted_x{ refractive_index * (vec - std::fmin(Vec3::dot(vec, normal), 1.f) * normal) };

		const Vec3 refracted_y{ -normal * std::sqrtf(std::fabs(1.0 - refracted_x.length_squared())) };
		
		return refracted_x + refracted_y;
	}

	/// \brief Calculates the Fresnel reflectance using Schlick's Approximation.
	/// \param cos_theta The cosine of the angle between the ray and the normal.
	/// \param refraction_index The refractive index of the material.
	/// \returns The reflectance value (0.0 to 1.0).
	[[nodiscard]] inline auto schlick_approximation(double cos_theta, double refraction_index) -> double
	{
		double r0{ (1.0 - refraction_index) / (1.0 + refraction_index) };
		r0 = r0 * r0;
		return r0 + (1.0 - r0) * std::pow(1.0 - cos_theta, 5.0);
	}
}