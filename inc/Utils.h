#pragma once
#include "Vec.h"
#include <limits>
#include <numbers>
#include <random>

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
}