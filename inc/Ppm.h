#pragma once
#include <string_view>
#include <cstdint>
#include <fstream>
#include "Color.h"

class PpmFile
{
public:
	struct Header
	{
		std::string_view format{ "P3" };
		uint32_t width{ 40 };
		uint32_t height{ 40 };
		float max_color{ 255.999f };
	};

	explicit PpmFile(std::string_view name, const Header& header);

	[[nodiscard]] auto is_open() const noexcept -> bool;

	auto write_color(const Color3& color) -> void;

private:
	Header m_header;
	std::ofstream m_stream;
	bool m_is_open;
};

