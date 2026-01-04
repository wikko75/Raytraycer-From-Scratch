#pragma once
#include "Color.h"

struct Material
{
	enum class Type
	{
		METAL, LAMBERTIAN, DIELECTRIC
	};

	Type type;
	Color3 albedo{ 0.f, 0.f, 0.f };
	double fuzz{ 0.0 };
	double refraction_index{ 0.0 };
};

