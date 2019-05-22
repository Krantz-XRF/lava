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
		lava::format::format_s(err_msg, "错误: \"", file, "\":", lava::format::decimal(line), ':', func, ": ", msg);
		throw std::runtime_error(err_msg);
#	else
		lava::format::format_io(std::cerr, "错误: \"", file, "\":", lava::format::decimal(line), ':', func, ": ", msg);
		std::quick_exit(1);
#	endif
	}
#endif

// `panic` is always provided
// when LAVA_DISABLE_PANIC is defined, `panic` does nothing
// this option is dangerous, for potential errors get silently ignored
#ifndef LAVA_DISABLE_PANIC
#	define panic(...) RaiseError(lava::format::format("致命错误：", __VA_ARGS__))
#else
#	define panic(...) static_cast<void>(0)
#endif

// `expects`, `ensures`, `invariant`, `unreachable`
// 4 useful assertions are defined below
#ifndef LAVA_DISABLE_ASSERT
#	define expects(cond, ...) AssertImpl(cond, lava::format::format(__VA_ARGS__), lava::PreCond)
#	define ensures(cond, ...) AssertImpl(cond, lava::format::format(__VA_ARGS__), lava::PostCond)
#	define invariant(cond, ...) AssertImpl(cond, lava::format::format(__VA_ARGS__), lava::InvarCond)
#	define unreachable(...) RaiseError(lava::format::format("执行到一处不可达代码：", __VA_ARGS__))

#	ifndef LAVA_DISABLE_EXCEPTION
// 表示这个函数仅可能由于断言失败而抛出异常
// 此时断言是开启的，并且没有指定LAVA_DISABLE_EXCEPTION，因此这个函数可能抛出异常
#		define assert_except noexcept(false)
#	else
// 表示这个函数仅可能由于断言失败而抛出异常
// 此时断言是开启的，断言不会抛出异常（指定了LAVA_DISABLE_EXCEPTION），因此这个函数不会抛出异常
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
		static const char* assert_type_name[]{"先置条件", "后置条件", "不变式"};
		std::string err_msg{};
		lava::format::format_s(err_msg, assert_type_name[type], '[', cond_str, ']', "未满足。错误信息：", msg);
		return err_msg;
	}

#else
#	define expects(cond, ...) static_cast<void>(0)
#	define ensures(cond, ...) static_cast<void>(0)
#	define invariant(cond, ...) static_cast<void>(0)
#	define unreachable(...) static_cast<void>(0)

// 表示这个函数仅可能由于断言失败而抛出异常
// 此时断言被关闭，因此这个函数不会抛出异常
#	define assert_except noexcept
#endif
} // namespace lava
