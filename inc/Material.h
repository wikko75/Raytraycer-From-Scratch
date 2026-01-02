#pragma once
#include <Color.h>

struct Material
{
	enum class Type
	{
		METAL, LAMBERTIAN
	};

	Type type;
	Color3 albedo{ 0.f, 0.f, 0.f };
};

