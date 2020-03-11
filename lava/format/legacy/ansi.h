#pragma once
#include "basic.h"
#include "integers.h"

namespace lava::format::legacy
{
	// Wrapper for ANSI Escape Sequence
	struct ansi
	{
		std::string ansi;
	};

	// Combine 2 ANSI Escape Sequences
	inline ansi operator|(const ansi& lhs, const ansi& rhs)
	{
		return ansi{lhs.ansi + ";" + rhs.ansi};
	}

	// Combine 2 ANSI Escape Sequences, alias for operator|
	inline ansi operator+(const ansi& lhs, const ansi& rhs)
	{
		return operator|(lhs, rhs);
	}

	inline const ansi Black{"30"};
	inline const ansi Red{"31"};
	inline const ansi Green{"32"};
	inline const ansi Blue{"34"};
	inline const ansi Yellow{"33"};
	inline const ansi Cyan{"36"};
	inline const ansi Magenta{"35"};
	inline const ansi White{"37"};
	inline const ansi Black_BRI{"90"};
	inline const ansi Red_BRI{"91"};
	inline const ansi Green_BRI{"92"};
	inline const ansi Blue_BRI{"94"};
	inline const ansi Yellow_BRI{"93"};
	inline const ansi Cyan_BRI{"96"};
	inline const ansi Magenta_BRI{"95"};
	inline const ansi White_BRI{"97"};
	inline const ansi Reset{"0"};
	inline const ansi Intense{"1"};
	inline const ansi Fainted{"2"};

	inline const ansi ErrorColour = Red_BRI + Intense;
	inline const ansi WarningColour = Yellow_BRI + Intense;
	inline const ansi InfoColour = Cyan_BRI + Intense;
	inline const ansi DebugColour = Blue_BRI + Intense;

	template<> // format a ANSI colour sequence
	struct format_trait<ansi>
	{
		static void format_append(std::string& res, const ansi& c)
		{
#ifndef LAVA_DISABLE_ANSI_ESCAPE_SEQUENCE
			format_s(res, "\033[", c.ansi, 'm');
#endif
		}
	};

#ifndef LAVA_DISABLE_ANSI_ESCAPE_MACROS
#	define mkAnsi(seq, ...) seq, __VA_ARGS__, lava::format::legacy::Reset
#	define fmtAnsi(...) format(mkAnsi(__VA_ARGS__))
#endif
} // namespace lava::format::legacy
