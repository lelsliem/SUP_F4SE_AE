#pragma once

// SUP string helpers — Phase 2 of the modernization (see MODERNIZATION_PLAN.md).
// Replaces the 9 boost::algorithm functions the codebase used (to_lower, iequals, icontains,
// contains, equals, replace_all, replace_first, replace_last) so Boost can be dropped entirely.

#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>

namespace sup::string
{
	inline void to_lower(std::string& a_str)
	{
		std::transform(a_str.begin(), a_str.end(), a_str.begin(), [](unsigned char a_c) {
			return static_cast<char>(std::tolower(a_c));
		});
	}

	[[nodiscard]] inline std::string to_lower_copy(const std::string_view a_str)
	{
		std::string result(a_str);
		to_lower(result);
		return result;
	}

	[[nodiscard]] inline bool iequals(const std::string_view a, const std::string_view b)
	{
		return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](unsigned char a_c, unsigned char b_c) {
			return std::tolower(a_c) == std::tolower(b_c);
		});
	}

	[[nodiscard]] inline bool contains(const std::string_view a_haystack, const std::string_view a_needle)
	{
		return a_haystack.find(a_needle) != std::string_view::npos;
	}

	[[nodiscard]] inline bool icontains(const std::string_view a_haystack, const std::string_view a_needle)
	{
		if (a_needle.empty()) {
			return true;
		}
		if (a_needle.size() > a_haystack.size()) {
			return false;
		}
		for (std::size_t pos = 0; pos + a_needle.size() <= a_haystack.size(); ++pos) {
			if (iequals(a_haystack.substr(pos, a_needle.size()), a_needle)) {
				return true;
			}
		}
		return false;
	}

	[[nodiscard]] inline bool equals(const std::string_view a, const std::string_view b)
	{
		return a == b;
	}

	inline void replace_all(std::string& a_str, const std::string_view a_old, const std::string_view a_new)
	{
		if (a_old.empty()) {
			return;
		}
		std::size_t pos = 0;
		while ((pos = a_str.find(a_old, pos)) != std::string::npos) {
			a_str.replace(pos, a_old.size(), a_new);
			pos += a_new.size();
		}
	}

	inline void replace_first(std::string& a_str, const std::string_view a_old, const std::string_view a_new)
	{
		const std::size_t pos = a_str.find(a_old);
		if (pos != std::string::npos) {
			a_str.replace(pos, a_old.size(), a_new);
		}
	}

	inline void replace_last(std::string& a_str, const std::string_view a_old, const std::string_view a_new)
	{
		const std::size_t pos = a_str.rfind(a_old);
		if (pos != std::string::npos) {
			a_str.replace(pos, a_old.size(), a_new);
		}
	}
}
