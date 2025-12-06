#pragma once

#include "Hittable.h"
#include "Interval.h"
#include "Ray.h"
#include <memory>
#include <optional>
#include <vector>

// Maybe use references for object and reference_wrapper for m_hittsbles? or just shared_ptr
class HittableList : public Hittable
{
public:
	HittableList() = default;

	explicit HittableList(std::unique_ptr<Hittable> object);
	
	// Inherited via Hittable
	[[nodiscard]] auto hit(const Ray& ray, const Interval& ray_interval) const -> std::optional<HitResult> override;

	auto add(std::unique_ptr<Hittable> object) -> void;
private:
	std::vector<std::unique_ptr<Hittable>> m_hittables;
};