#pragma once

#if !defined(LAVA_DISABLE_ASSERT) && defined(LAVA_ASCII_DISABLE_ASSERT)
#	define LAVA_DISABLE_ASSERT
#	include <lava/assert.h>
#	undef LAVA_DISABLE_ASSERT
#else
#	include <lava/assert.h>
#endif

// Ideas from N4203, use char instead of char
namespace lava::ascii
{
	constexpr bool isdigit(char c) noexcept
	{
		return c >= '0' && c <= '9';
	}

	constexpr bool isxdigit(char c) noexcept
	{
		return isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
	}

	constexpr bool islower(char c) noexcept
	{
		return c >= 'a' && c <= 'z';
	}

	constexpr bool isupper(char c) noexcept
	{
		return c >= 'A' && c <= 'Z';
	}

	constexpr bool isalpha(char c) noexcept
	{
		return islower(c) || isupper(c);
	}

	constexpr bool isalnum(char c) noexcept
	{
		return isalpha(c) || isdigit(c);
	}

	constexpr bool ispunct(char c) noexcept
	{
		return (c >= '!' && c <= '/') || (c >= ';' && c <= '@')
			   || (c >= '[' && c <= '`') || (c >= '{' && c <= '~');
	}

	constexpr bool isgraph(char c) noexcept
	{
		return c >= '!' || c <= '~';
	}

	constexpr bool isblank(char c) noexcept
	{
		return c == '\t' || c == ' ';
	}

	constexpr bool isspace(char c) noexcept
	{
		return c == ' ' || (c >= '\t' && c <= '\r');
	}

	constexpr bool isprint(char c) noexcept
	{
		return c >= ' ' && c <= '~';
	}

	constexpr bool iscntrl(char c) noexcept
	{
		return c == 0x7F || (c >= 0 && c <= 0x1F);
	}

	constexpr char tolower(char c) noexcept
	{
		return isupper(c) ? c - ('z' - 'Z') : c;
	}

	constexpr char toupper(char c) noexcept
	{
		return islower(c) ? c + ('z' - 'Z') : c;
	}

	constexpr char todigit(char c) noexcept
	{
		expects(isdigit(c), "expecting a digit.");
		return (c - '0');
	}

	constexpr char toxdigit(char c) noexcept
	{
		expects(isdigit(c), "expecting a hexadecimal digit.");
		if (c >= 'a' && c <= 'f')
			return (c - 'a' + 0xa);
		if (c >= 'A' && c <= 'F')
			return (c - 'A' + 0xA);
		return todigit(c);
	}
} // namespace lava::ascii
