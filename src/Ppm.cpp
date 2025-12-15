#include "Ppm.h"
#include <print>
#include <fstream>
#include <format>
#include "Color.h"
#include <string_view>
#include "Interval.h"
#include <cmath>

PpmFile::PpmFile(std::string_view name, const Header& header)
	: m_header{ header }
	, m_is_open{ false }
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
	constexpr Interval normalized_color{ 0.0, 0.9999 };
	const Color3 gamma_corrected{ to_gamma_space(color) };

	m_stream << static_cast<int>(m_header.max_color * normalized_color.clamp(gamma_corrected.r()))
		<< ' '
		<< static_cast<int>(m_header.max_color * normalized_color.clamp(gamma_corrected.g()))
		<< ' '
		<< static_cast<int>(m_header.max_color * normalized_color.clamp(gamma_corrected.b()))
		<< '\n';
}

auto PpmFile::to_gamma_space(const Color3& color) const -> Color3
{
	return Color3{ 
		color.r() > 0 ? std::sqrt(color.r()) : 0,
		color.g() > 0 ? std::sqrt(color.g()) : 0,
		color.b() > 0 ? std::sqrt(color.b()) : 0 
	};
}
