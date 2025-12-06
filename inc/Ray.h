#pragma once
#include "Vec.h"

class Ray
{
public:
	explicit Ray(const Vec3& origin, const Vec3& direction)
		: m_origin{ origin }
		, m_direction{ direction }
	{
	}

	[[nodiscard]] auto get_origin() const noexcept -> const Vec3&
	{
		return m_origin;
	}

	[[nodiscard]] auto get_direction() const noexcept -> const Vec3&
	{
		return m_direction;
	}

	[[nodiscard]] auto at(const float t) const noexcept -> Vec3
	{
		return m_origin + (t * m_direction);
	}

private:
	Vec3 m_origin;
	Vec3 m_direction;
};
