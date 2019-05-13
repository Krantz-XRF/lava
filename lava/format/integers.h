#pragma once
#include "basic.h"
#include <algorithm>

namespace lava::format
{
	template<typename T, T base = 10, bool capital = true>
	struct num_base // format a integer using the given base
	{
		static_assert(1 < base && base <= 36, "Base allowed in [2, 36].");
		T value;
	};
	// helper function `number`
	template<long long base = 10, bool capital = true, typename T = long long>
	constexpr num_base<T, T(base), capital> number(T x)
	{
		return {x};
	}

#define DEFINE_NUM_BASE(name, base)                \
	template<bool capital = true, typename T>      \
	constexpr num_base<T, base, capital> name(T x) \
	{                                              \
		return {x};                                \
	}
	// define helper functions for the 4 bases: 2, 8, 10, 16
	DEFINE_NUM_BASE(hexadecimal, 16)
	DEFINE_NUM_BASE(decimal, 10)
	DEFINE_NUM_BASE(octal, 8)
	DEFINE_NUM_BASE(binary, 2)
#undef DEFINE_NUM_BASE

	template<typename T, T base, bool capital> // format a integer with specified base
	struct format_trait<num_base<T, base, capital>>
	{
		using U = num_base<T, base, capital>;
		static char digit_of(T x)
		{
			if (x < 10) // 0-9
				return static_cast<char>(x + '0');
			if constexpr (capital) // A-Z
				return static_cast<char>(x - 10 + 'A');
			else // a-z
				return static_cast<char>(x - 10 + 'a');
		}

		static std::string format_str(U ux)
		{
			T x = ux.value;
			std::string res{};
			if constexpr (std::is_signed_v<T>)
				if (x < 0)
					x = -x;
			do
			{
				res.push_back(digit_of(x % base));
				x /= base;
			} while (x != 0);
			if constexpr (std::is_signed_v<T>)
				if (x < 0)
					res.push_back('-');
			std::reverse(res.begin(), res.end());
			return res;
		}
		static void format_append(std::string& res, U x) { res.append(format_str(x)); }
		static void format_stream(std::ostream& os, U x) { os << format_str(x); }
	};
} // namespace lava::format
