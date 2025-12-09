#include "Ppm.h"
#include <print>
#include <fstream>
#include <format>
#include "Color.h"
#include <string_view>
#include "Interval.h"

PpmFile::PpmFile(std::string_view name, const Header& header)
	: m_header{header}
	, m_is_open{false}
{
	m_stream = std::ofstream(name.data(), std::ios::binary);
	
	if (!m_stream.is_open())
	{
		std::print("Can't open file to save!\n");
	}

	m_is_open = true;
	m_stream << std::format("{}\n{} {}\n{}\n", header.format, header.width, header.height, static_cast<int>(header.max_color));
}

auto PpmFile::is_open() const noexcept -> bool
{
	return m_is_open;
}

auto PpmFile::write_color(const Color3& color) -> void
{
	constexpr Interval normalized_color{0.0, 0.9999};

	m_stream << static_cast<int>(m_header.max_color * normalized_color.clamp(color.r()))
		<< ' '
		<< static_cast<int>(m_header.max_color * normalized_color.clamp(color.g()))
		<< ' '
		<< static_cast<int>(m_header.max_color * normalized_color.clamp(color.b()))
		<< '\n';
}
