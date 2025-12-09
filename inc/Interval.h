#pragma once
#include "Utils.h"

class Interval
{
public:
	constexpr Interval(double min, double max)
		: m_min{ min }
		, m_max{ max }
	{
	}

	constexpr Interval()
		: m_min{ constants::infinity }
		, m_max{ -constants::infinity }
	{
	}

	[[nodiscard]] constexpr auto size() const -> double
	{
		return m_max - m_min;
	}

	[[nodiscard]] constexpr auto contains(double t) const -> bool
	{
		return (m_min <= t) && (t <= m_max);
	}

	[[nodiscard]] constexpr auto surrounds(double t) const -> bool
	{
		return (m_min < t) && (t < m_max);
	}

	[[nodiscard]] constexpr auto clamp(double value) const -> double
	{
		if (value < m_min)
			return m_min;
		
		if (value > m_max)
			return m_max;
		
		return value;
	}

	[[nodiscard]] constexpr auto get_max() const -> double
	{
		return m_max;
	}

	[[nodiscard]] constexpr auto get_min() const -> double
	{
		return m_min;
	}

	static const Interval empty;
	static const Interval universe;

private:
	double m_min;
	double m_max;
};

inline const Interval Interval::empty{ Interval{} };
inline const Interval Interval::universe{ Interval{ -constants::infinity, constants::infinity } };

