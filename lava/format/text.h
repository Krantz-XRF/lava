#pragma once
#include "basic.h"
#include "meta.h"

namespace lava::format
{
	template<> // format a single character
	struct format_trait<char>
	{
		static std::string format_str(char c) { return std::string(1, c); }
		static void format_append(std::string& res, char c) { res.push_back(c); }
		static void format_stream(std::ostream& os, char c) { os.put(c); }
	};

	template<> // format a C-style NUL-terminated string
	struct format_trait<const char*>
	{
		static std::string format_str(const char* s) { return {s}; }
		static void format_append(std::string& res, const char* s) { res.append(s); }
		static void format_stream(std::ostream& os, const char* s) { os << s; }
	};

	template<> // format a C++ std::string
	struct format_trait<std::string>
	{
		static std::string format_str(const std::string& s) { return {s}; }
		static void format_append(std::string& res, const std::string& s) { res.append(s); }
		static void format_stream(std::ostream& os, const std::string& s) { os << s; }
	};

	// format structure for a unicode character
	struct unicode_t
	{
		char32_t value;
	};
	constexpr unicode_t unicode(char32_t c) { return unicode_t{c}; }

	template<> // format a unicode character as U+XXXX[D]
	struct format_trait<unicode_t>
	{
#define UCHAR_FORMAT_SEQ(uc) \
	"U+", fill(right(4, '0'), hexadecimal(uint32_t(uc))), '[', decimal(uint32_t(uc)), ']'
		static std::string format_str(unicode_t c)
		{
			return format(UCHAR_FORMAT_SEQ(c.value));
		}
		static void format_append(std::string& res, unicode_t c)
		{
			format_s(res, UCHAR_FORMAT_SEQ(c.value));
		}
		static void format_stream(std::ostream& os, unicode_t c)
		{
			format_io(os, UCHAR_FORMAT_SEQ(c.value));
		}
#undef UCHAR_FORMAT_SEQ
	};
} // namespace lava::format
