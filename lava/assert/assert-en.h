#pragma once
#include <lava/format.h>
#include <stdexcept>

// if exceptions are disabled, error messages are printed to `std::cerr`
#ifdef LAVA_DISABLE_EXCEPTION
#	include <iostream>
#endif

namespace lava
{
	enum AssertionType
	{
		PreCond,
		PostCond,
		InvarCond
	};

// `assert` and `panic` both depends on macro `RaiseError`
// so if either is enabled, `RaiseError` should be defined
// user should not call `RaiseError` directly
// for it is not defined when ASSERT and PANIC are both disabled
#if !defined(LAVA_DISABLE_PANIC) || !defined(LAVA_DISABLE_ASSERT)
#	define RaiseError(msg) lava::RaiseErrorImpl(__FILE__, __LINE__, __func__, msg)
	[[noreturn]] inline void RaiseErrorImpl(const char* file, int line, const char* func, const std::string& msg)
	{
#	ifndef LAVA_DISABLE_EXCEPTION
		std::string err_msg{};
		lava::format::format_s(err_msg, "error: \"", file, "\":", lava::format::decimal(line), ':', func, ": ", msg);
		throw std::runtime_error(err_msg);
#	else
		lava::format::format_io(std::cerr, "error: \"", file, "\":", lava::format::decimal(line), ':', func, ": ", msg);
		std::quick_exit(1);
#	endif
	}
#endif

// `panic` is always provided
// when LAVA_DISABLE_PANIC is defined, `panic` does nothing
// this option is dangerous, for potential errors get silently ignored
#ifndef LAVA_DISABLE_PANIC
#	define panic(...) RaiseError(lava::format::format("panic£º", __VA_ARGS__))
#else
#	define panic(...) static_cast<void>(0)
#endif

// `expects`, `ensures`, `invariant`, `unreachable`
// 4 useful assertions are defined below
#ifndef LAVA_DISABLE_ASSERT
#	define expects(cond, ...) AssertImpl(cond, lava::format::format(__VA_ARGS__), lava::PreCond)
#	define ensures(cond, ...) AssertImpl(cond, lava::format::format(__VA_ARGS__), lava::PostCond)
#	define invariant(cond, ...) AssertImpl(cond, lava::format::format(__VA_ARGS__), lava::InvarCond)
#	define unreachable(...) RaiseError(lava::format::format("unreachable codes are reached£º", __VA_ARGS__))

#	ifndef LAVA_DISABLE_EXCEPTION
// this function throws only when assertions fail in function body
// assertions are enabled, and LAVA_DISABLE_EXCEPTION is not defined, so this function may throw
#		define assert_except noexcept(false)
#	else
// this function throws only when assertions fail in function body
// assertions are enabled, but LAVA_DISABLE_EXCEPTION is defined, so this function will never throw
#		define assert_except noexcept
#	endif

#	define AssertImpl(cond, msg, type)                             \
		do                                                          \
		{                                                           \
			if (!(cond))                                            \
				RaiseError(lava::AssertionError(#cond, msg, type)); \
		} while (0)
	inline std::string
		AssertionError(const std::string& cond_str, const std::string& msg, lava::AssertionType type)
	{
		static const char* assert_type_name[]{"pre-condition", "post-condition", "invariant"};
		std::string err_msg{};
		lava::format::format_s(err_msg, assert_type_name[type], '[', cond_str, ']', " is not satisfied. message£º", msg);
		return err_msg;
	}

#else
#	define expects(cond, ...) static_cast<void>(0)
#	define ensures(cond, ...) static_cast<void>(0)
#	define invariant(cond, ...) static_cast<void>(0)
#	define unreachable(...) static_cast<void>(0)

// this function throws only when assertions fail in function body
// assertions are disabled, so this function will never throw
#	define assert_except noexcept
#endif
} // namespace lava
