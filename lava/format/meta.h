#pragma once
#include "basic.h"
#include <string>

namespace lava::format
{
	// 3 type of alignments
	enum class alignment : unsigned char
	{
		left,
		center,
		right
	};
	// the `align_t` struct for format_*
	struct align_t
	{
		int width;
		alignment align;
		char to_fill;
	};
#define DEFINE_ALIGNMENT(align, name) \
	constexpr inline align_t name(int width, char to_fill = ' ') { return {width, align, to_fill}; }
	// define all three shortcut functions
	DEFINE_ALIGNMENT(alignment::left, left)
	DEFINE_ALIGNMENT(alignment::right, right)
	DEFINE_ALIGNMENT(alignment::center, center)
#undef DEFINE_ALIGNMENT

	// align the contents and fill the blank
	// note that this trait will force the content be formatted as string first
	// this implies that every layer of `fill_t` introduces a full traverse on the content
	struct fill_t
	{
		std::string content;
		int width;
		alignment align;
		char to_fill;
	};
	template<> // fill the string to specified width
	struct format_trait<fill_t>
	{
		static std::string format_str(fill_t&& f)
		{
			const int l = (int) f.content.length();
			if (f.width <= l)
				return std::move(f.content);

			std::string res{};
			res.reserve(f.width);

			int l_cnt{0}, r_cnt{0};
			const int delta = f.width - l;
			if (f.align == alignment::left)
				r_cnt = delta;
			else if (f.align == alignment::right)
				l_cnt = delta;
			else
			{
				l_cnt = delta / 2;
				r_cnt = (delta + 1) / 2;
			}

			for (int i{0}; i < l_cnt; ++i)
				res.push_back(f.to_fill);
			res.append(f.content);
			for (int i{0}; i < r_cnt; ++i)
				res.push_back(f.to_fill);

			return res;
		}
		static void format_append(std::string& res, fill_t&& f)
		{
			res.append(format_str(std::move(f)));
		}
		static void format_stream(std::ostream& os, fill_t&& f) { os << format_str(std::move(f)); }
	};
	// helper function for format `fill`
	template<typename... Us>
	fill_t fill(align_t align, Us&&... x)
	{
		return fill_t{format(std::forward<Us>(x)...), align.width, align.align, align.to_fill};
	}
} // namespace lava::format