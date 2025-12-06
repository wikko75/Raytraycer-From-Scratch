#pragma once

#include <format>
#include <concepts>
#include <tuple>

template<typename ...Args>
	requires (std::floating_point<Args> && ...)
class Vec
{
public:
	explicit constexpr Vec(Args... args)
		: m_data(args...)
	{ }

	constexpr auto operator+=(const Vec& lhs) noexcept -> Vec&
	{
		m_data = std::apply([&lhs](const auto&... m_data_elem) 
			{
				return std::apply([&m_data_elem...](const auto&...lhs_elem)
					{
						return std::tuple{ (m_data_elem + lhs_elem)... };
					},lhs.get_args());
		},m_data);

		return *this;
	}

	constexpr auto operator-=(const Vec& lhs) noexcept -> Vec&
	{
		m_data = std::apply([&lhs](const auto&... m_data_elem)
			{
				return std::apply([&m_data_elem...](const auto&...lhs_elem)
					{
						return std::tuple{ (m_data_elem - lhs_elem)... };
					}, lhs.get_args());
			}, m_data);

		return *this;
	}

	constexpr auto operator*=(const Vec& lhs) noexcept -> Vec&
	{
		m_data = std::apply([&lhs](const auto&... m_data_elem)
			{
				return std::apply([&m_data_elem...](const auto&...lhs_elem)
					{
						return std::tuple{ (m_data_elem * lhs_elem)... };
					}, lhs.get_args());
			}, m_data);

		return *this;
	}

	constexpr auto operator*=(const float scalar) noexcept -> Vec&
	{
		m_data = std::apply([&scalar](const auto&... m_data_elem) {
				return std::tuple{ (m_data_elem * scalar)... };
			}, m_data);

		return *this;
	}

	constexpr auto operator/=(float scalar) noexcept -> Vec&
	{
		return *this *= (1 / scalar);
	}

	constexpr auto operator-() const noexcept -> Vec
	{
		return std::apply([](auto&... args) {
			return Vec((-args)...);
			}, m_data);
	}

	constexpr auto operator+(const Vec& rhs) const noexcept -> Vec
	{
		Vec temp{ *this };
		temp += rhs;
		return temp;
	}
	constexpr auto operator-(const Vec& rhs) const noexcept -> Vec
	{
		Vec temp{ *this };
		temp -= rhs;
		return temp;
	}

	constexpr auto operator*(const Vec& rhs) const noexcept -> Vec
	{
		Vec temp{ *this };
		temp *= rhs;
		return temp;
	}

	constexpr auto operator*(const float scalar) const noexcept -> Vec
	{
		Vec temp{ *this };
		temp *= scalar;
		return temp;
	}

	friend constexpr auto operator*(const float scalar, const Vec& rhs) noexcept -> Vec
	{
		return rhs * scalar;
	}

	constexpr auto operator/(float scalar) const noexcept -> Vec
	{
		Vec temp{ *this };
		temp /= scalar;
		return temp;
	}

	constexpr auto operator[](size_t idx) const -> decltype(auto)
	{
		return std::get<idx>(m_data);
	}
	// idx must be compile constant
	//constexpr auto& operator[](size_t idx) &
	//{
	//	return std::get<idx>(m_data);
	//}


	[[nodiscard]] auto x() const -> decltype(auto)
	{
		return std::get<0>(m_data);
	}

	[[nodiscard]] auto y() const -> decltype(auto)
	{
		return std::get<1>(m_data);
	}

	[[nodiscard]] auto z() const -> decltype(auto)
	{
		return std::get<2>(m_data);
	}

	[[nodiscard]] auto r() const -> decltype(auto)
	{
		return std::get<0>(m_data);
	}

	[[nodiscard]] auto g() const -> decltype(auto)
	{
		return std::get<1>(m_data);
	}

	[[nodiscard]] auto b() const -> decltype(auto)
	{
		return std::get<2>(m_data);
	}

	[[nodiscard]] auto length() const -> float{
		return std::sqrt(length_squared());
	}

	[[nodiscard]] auto length_squared() const -> decltype(auto)
	{
		return std::apply([](const auto&... args) {
			return ((args * args) + ...);
			}, m_data);
	}

	[[nodiscard]] auto unit_vector() const -> Vec
	{
		return *this / length();
	}

	[[nodiscard]] static auto dot(const Vec& lhs, const Vec& rhs) -> float
	{
		return std::apply([&rhs](const auto&... lhs_elems) {
			return std::apply([&lhs_elems...](const auto&... rhs_elems) {
				return ((lhs_elems * rhs_elems) + ...);
			}, rhs.get_args());
		}, lhs.get_args());
	}

	/*
	WARNING: works ONLY for 3 dimensional Vec
	*/
	static auto cross(const Vec& u, const Vec& v) -> Vec
	{
		return Vec{ 
			(u.y() * v.z()) - (u.z() * v.y()),
			(u.z() * v.x()) - (u.x() * v.z()),
			(u.x() * v.y()) - (u.y() * v.x())
		};
	}

	friend std::formatter<Vec>;

private:
	[[nodiscard]] constexpr auto get_args() const -> auto&
	{
		return m_data;
	}

	std::tuple<Args...> m_data{};
};



template <typename... Args>
struct std::formatter<Vec<Args...>> {
	constexpr auto parse(std::format_parse_context& ctx) {
		return ctx.begin();
	}

	auto format(const Vec<Args...>& obj, std::format_context& ctx) const {
		return std::format_to(ctx.out(), "{}", obj.get_args());
	}
};


using Point3 = Vec<float,float,float>;
using Vec3 = Vec<float, float, float>;

