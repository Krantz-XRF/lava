#pragma once
#include <lava/format.h>
#include <string>

namespace lava::trace
{
	template<typename T>
	const char* get_type_helper()
	{
#if defined(__GNUC__) || defined(__clang__)
		return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
		return __FUNCSIG__;
#else
#	error "No support for this compiler."
#endif
	}

	template<typename T>
	std::string get_type()
	{
		std::string signature = get_type_helper<T>();
#if defined(_MSC_VER)
		// "const char *__cdecl lava::trace::get_type_helper<*>(void)"
		constexpr size_t prefix_length = sizeof("const char *__cdecl lava::trace::get_type_helper<") - 1;
		constexpr size_t suffix_length = sizeof(">(void)") - 1;
#elif defined(__clang__)
		// "const char* lava::trace::get_type_helper() [T = *]"
		constexpr size_t prefix_length = sizeof("const char* lava::trace::get_type_helper() [T = ") - 1;
		constexpr size_t suffix_length = sizeof("]") - 1;
#elif defined(__GNUC__)
		// "const char* lava::trace::get_type_helper() [with T = *]"
		constexpr size_t prefix_length = sizeof("const char* lava::trace::get_type_helper() [with T = ") - 1;
		constexpr size_t suffix_length = sizeof("]") - 1;
#else
#	error "Compiler not supported."
#endif
		return signature.substr(prefix_length, signature.size() - prefix_length - suffix_length);
	}

	void trace_message(const std::string& str);

	template<typename T, typename F>
	T&& debug_trace(const char* file, int line, const char* func, const char* expr, T&& val, F&& fmt)
	{
		std::string msg = format::format(
			'[', file, ':', format::decimal(line), " (", func, ")] ",
			'(', get_type<T>(), ") ", expr, " = ", std::forward<F>(fmt),
			format::endl);
		trace_message(msg);
		return std::forward<T>(val);
	}

	template<typename T, typename C, size_t N, typename = std::enable_if_t<format::is_char<C>>>
	T&& debug_trace(const char* file, int line, const char* func, const char* expr, T&& val, const C (&fmt)[N])
	{
		std::string msg = format::format('[', file, ':', format::decimal(line), " (", func, ")] ", fmt, format::endl);
		trace_message(msg);
		return std::forward<T>(val);
	}

	template<typename T>
	using literal = std::conditional_t<
		format::is_char<T> || format::is_string<T>,
		format::literal<T>,
		format::identity<T>>;

	template<typename T>
	literal<T> lit(T&& v)
	{
		return std::forward<T>(v);
	}

	template<typename C, size_t N, typename = std::enable_if_t<format::is_char<C>>>
	auto lit(const C (&v)[N]) -> const C (&)[N]
	{
		return v;
	}

#define trace(...) \
	lava::trace::debug_trace(__FILE__, __LINE__, __func__, #__VA_ARGS__, (__VA_ARGS__), lava::trace::lit(__VA_ARGS__))
#define traceShow(show, ...) \
	lava::trace::debug_trace(__FILE__, __LINE__, __func__, #__VA_ARGS__, (__VA_ARGS__), show(__VA_ARGS__))
#define setTraceOutput(...) \
	void lava::trace::trace_message(const std::string& msg) { (__VA_ARGS__) << msg; }
} // namespace lava::trace
