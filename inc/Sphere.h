#pragma once

#include "Hittable.h"
#include "Interval.h"
#include "Ray.h"
#include "Vec.h"
#include <optional>

class Sphere : public Hittable
{
public:
	Sphere(const Vec3& position, float radius);
	// Inherited via Hittable
	[[nodiscard]] auto hit(const Ray& ray, const Interval& ray_interval) const -> std::optional<HitResult> override;

	~Sphere() = default;

private:
	Vec3 m_position;
	float m_radius;
};
