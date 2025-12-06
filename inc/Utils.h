#pragma once
#include <limits>
#include <numbers>


namespace constants {

	constexpr double pi{ std::numbers::pi };
	constexpr double infinity{ std::numeric_limits<double>::infinity() };

}

namespace utility_functions {
	constexpr auto radians(double degrees) -> double
	{
		return (degrees / 180) * constants::pi;
	}

	constexpr auto degrees(double radians) -> double
	{
		return (radians / constants::pi) * 180;
	}
}