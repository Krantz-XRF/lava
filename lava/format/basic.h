#pragma once
#include <ostream>
#include <string>

namespace lava::format
{
	// trait format_trait<T>
	template<typename T>
	struct format_trait;

	inline void format_s(std::string& res) {}
	// format_s: format all the parameters to string `res`
	template<typename U, typename... Us>
	inline void format_s(std::string& res, U&& x, Us&&... xs)
	{
		format_trait<std::decay_t<U>>::format_append(res, std::forward<U>(x));
		format_s(res, std::forward<Us>(xs)...);
	}

	// format: format all the parameters, return the result
	template<typename... Us>
	inline std::string format(Us&&... args)
	{
		std::string res{};
		format_s(res, std::forward<Us>(args)...);
		return res;
	}

	// format_io: format all the parameters to output stream `os`
	template<typename... Us>
	inline void format_io(std::ostream& os, Us&&... xs)
	{
		os << format(std::forward<Us>(xs)...);
	}

	// an example for supporting new types in lava.format
	// tag `endl` will introduce a LF to the format target
	struct endl_t
	{
	} endl;
	template<>
	struct format_trait<endl_t>
	{
		static void format_append(std::string& res, endl_t) { res.push_back('\n'); }
	};
} // namespace lava::format
