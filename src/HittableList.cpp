#include "Hittable.h"
#include "HittableList.h"
#include "Interval.h"
#include "Ray.h"
#include <memory>
#include <optional>
#include <utility>

HittableList::HittableList(std::unique_ptr<Hittable> object)
{
	m_hittables.emplace_back(std::move(object));
}

auto HittableList::hit(const Ray& ray, const Interval& ray_interval) const -> std::optional<HitResult>
{
	double nearest_hit{ ray_interval.get_max() };
	std::optional<HitResult> hit_result{};

	for (const auto& object : m_hittables)
	{
		const auto object_hit_result{ object.get()->hit(ray, Interval{ray_interval.get_min(), nearest_hit})};

		if (object_hit_result.has_value() && object_hit_result.value().t < nearest_hit)
		{
			nearest_hit = object_hit_result.value().t;
			hit_result = object_hit_result;
		}
	}

	return hit_result;
}

auto HittableList::add(std::unique_ptr<Hittable> object) -> void
{
	m_hittables.emplace_back(std::move(object));
}
