#pragma once
#include "basic.h"
#include "integers.h"
#include "meta.h"

namespace lava::format
{
	template<> // format a single character
	struct format_trait<char>
	{
		static void format_append(std::string& res, char c) { res.push_back(c); }
	};

	template<> // format a C-style NUL-terminated string
	struct format_trait<const char*>
	{
		static void format_append(std::string& res, const char* s) { res.append(s); }
	};

	template<> // format a non-const C-style NUL-terminated string
	struct format_trait<char*>
	{
		static void format_append(std::string& res, const char* s) { res.append(s); }
	};

	template<> // format a C++ std::string
	struct format_trait<std::string>
	{
		static void format_append(std::string& res, const std::string& s) { res.append(s); }
	};

	template<typename T>
	void to_literal(T c, std::string& res)
	{
		if (c == '\a')
			res.append("\\a");
		else if (c == '\b')
			res.append("\\b");
		else if (c == '\f')
			res.append("\\f");
		else if (c == '\n')
			res.append("\\n");
		else if (c == '\r')
			res.append("\\r");
		else if (c == '\t')
			res.append("\\t");
		else if (c == '\v')
			res.append("\\v");
		else if (c == '\\')
			res.append("\\\\");
		else if (c == '\'')
			res.append("\\'");
		else if (c == '\"')
			res.append("\\\"");
		else if (c == '\0')
			res.append("\\0");
		else
			res.push_back(c);
	}

	template<typename T>
	constexpr bool is_char
		= std::is_same_v<T, char> | std::is_same_v<T, char16_t> | std::is_same_v<T, char32_t>;

	template<typename T>
	struct is_string_helper : std::false_type
	{};
	template<typename T>
	struct is_string_helper<std::basic_string<T>>
	{
		static constexpr std::enable_if_t<is_char<T>, bool> value = true;
	};
	template<typename T>
	constexpr bool is_string = is_string_helper<T>::value;

	template<typename T>
	struct literal
	{
		T text;
		literal() = default;
		literal(T v)
			: text{std::move(v)}
		{}
	};
	template<typename C, size_t N>
	literal(const C (&)[N])->literal<std::basic_string<C>>;

	template<typename T>
	struct format_trait<literal<T>>
	{
		static std::enable_if_t<is_char<T>> format_append(std::string& res, literal<T> c)
		{
			res.push_back('\'');
			to_literal(c.text, res);
			res.push_back('\'');
		}
	};

	template<typename T>
	struct format_trait<literal<std::basic_string<T>>>
	{
		using str = literal<std::basic_string<T>>;
		static std::enable_if_t<is_char<T>> format_append(std::string& res, const str& s)
		{
			res.push_back('"');
			for (auto c : s.text)
				to_literal(c, res);
			res.push_back('"');
		}
	};

	// format structure for a unicode character
	struct unicode
	{
		char32_t value;
		unicode() = default;
		unicode(char32_t c)
			: value{c}
		{}
	};

	template<> // format a unicode character as U+XXXX[D]
	struct format_trait<unicode>
	{
#define UCHAR_FORMAT_SEQ(uc) \
	"U+", right(4, '0', hexadecimal(uint32_t(uc))), '[', decimal(uint32_t(uc)), ']'
		static void format_append(std::string& res, unicode c)
		{
			format_s(res, UCHAR_FORMAT_SEQ(c.value));
		}
#undef UCHAR_FORMAT_SEQ
	};
} // namespace lava::format
