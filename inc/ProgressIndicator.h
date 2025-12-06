#pragma once

#include <cstdint>
#include <string>
#include <print>

struct ProgressIndicator
{
	explicit ProgressIndicator(uint32_t scale, char character = '=')
		: m_data(scale + 2, ' ')
		, m_character{ character }
	{
		m_data[0] = '[';
		m_data[m_data.size() - 1] = ']';
	}

	void update(const float fraction)
	{
		m_data[static_cast<size_t>(1 + ( fraction * (m_data.size() - 2)) )] = m_character;
	}

	void show()
	{
		std::print("Progress: {}\n", m_data);
	}

private:
	std::string m_data;
	char m_character;
};