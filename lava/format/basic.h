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

	inline void format_io(std::ostream& os) {}
	// format_io: format all the parameters to output stream `os`
	template<typename U, typename... Us>
	inline void format_io(std::ostream& os, U&& x, Us&&... xs)
	{
		format_trait<std::decay_t<U>>::format_stream(os, std::forward<U>(x));
		format_io(os, std::forward<Us>(xs)...);
	}

	// default_format_trait: the default implementation for `format_trait`
	template<typename T>
	struct default_format_trait
	{
		using derived = format_trait<std::decay_t<T>>;
		static std::string format_str(T v) { return derived::format_append("", v); }
		static void format_append(std::string& res, T v) { res.append(derived::format_str(v)); }
		static void format_stream(std::ostream& os, T v) { os << derived::format_str(v); }
	};

// help macros
// begin the definition of a `format_trait`
#define FORMAT_TRAIT_BEGIN(type)                                  \
	template<>                                                    \
	struct format_trait<type> : public default_format_trait<type> \
	{
// begin the definition of a `format_trait` for a tag object `tag`
#define FORMAT_TRAIT_TAG_BEGIN(tag) FORMAT_TRAIT_BEGIN(tag##_t)

// format T to string
#define FORMAT_STRING(...) static std::string format_str(__VA_ARGS__)
// format T, and append to a string
#define FORMAT_STREAM(...) static void format_stream(__VA_ARGS__)
// format T, print to the output stream
#define FORMAT_APPEND(...) static void format_append(__VA_ARGS__)

// macros below will end the definition of a `format_trait`
// clang-format off
#define FORMAT_TRAIT_END() };
#define FORMAT_TRAIT_TAG_END() };
// helper macros for defining a tag object
#define SINGLETON_TAG(name) constexpr struct name##_t {} name
#define TAG_TYPE(name) name##_t
	// clang-format on

	// an example for using the above macros
	// tag `endl` will introduce a LF to the format target
	SINGLETON_TAG(endl);
	FORMAT_TRAIT_TAG_BEGIN(endl)
		FORMAT_STRING(endl_t) { return "\n"; }
		FORMAT_STREAM(std::ostream & os, endl_t) { std::endl(os); }
		FORMAT_APPEND(std::string & res, endl_t) { res.push_back('\n'); }
	FORMAT_TRAIT_TAG_END()
} // namespace lava::format
