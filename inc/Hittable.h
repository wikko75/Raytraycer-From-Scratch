#pragma once
#include "Ray.h"
#include "Vec.h"
#include <optional>
#include "Interval.h"

// TODO: CRTP(Curiously Recurring Template Pattern) instead of runtime polimorphism
class Hittable
{
public:

	struct HitResult
	{
		float t;
		Vec3 hit_point;
		Vec3 normal;
		bool front_face;
		
		auto set_face_normal(const Ray& ray, const Vec3& outward_normal) -> void
		{
			// test if ray hits inside of sphere -> ensure normal always point out
			front_face = Vec3::dot(outward_normal, ray.get_direction()) < 0;
			normal = front_face ? outward_normal : -outward_normal;
		}
	};

	virtual ~Hittable() = default;

	[[nodiscard]] virtual auto hit(const Ray& ray, const Interval& ray_interval) const -> std::optional < HitResult > = 0;
};

