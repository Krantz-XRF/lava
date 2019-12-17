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
		static void format_append(std::string& res, fill_t&& f)
		{
			const int l = (int) f.content.length();
			if (f.width <= l)
			{
				res.append(f.content);
				return;
			}

			res.reserve(res.length() + f.width);

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
		}
	};

#define DEFINE_ALIGNMENT(align, name)                                    \
	template<typename... Us>                                             \
	inline fill_t name(int width, Us&&... xs)                            \
	{                                                                    \
		return {format(std::forward<Us>(xs)...), width, align, ' '};     \
	}                                                                    \
	template<typename... Us>                                             \
	inline fill_t name##_fill(int width, char to_fill, Us&&... xs)       \
	{                                                                    \
		return {format(std::forward<Us>(xs)...), width, align, to_fill}; \
	}
	// define all three shortcut functions
	DEFINE_ALIGNMENT(alignment::left, left)
	DEFINE_ALIGNMENT(alignment::right, right)
	DEFINE_ALIGNMENT(alignment::center, center)
#undef DEFINE_ALIGNMENT
} // namespace lava::format