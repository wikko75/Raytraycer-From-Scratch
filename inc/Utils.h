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
}