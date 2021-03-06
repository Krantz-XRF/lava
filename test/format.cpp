#include <iostream>
#include <lava/format/legacy.h>

int main()
{
	namespace fmt = lava::format::legacy;
	int arr[] = {42, 0, 1};
	fmt::format_io(
		std::cout,
		"String:         ", "A String", fmt::endl,
		"Character:      ", 'a', fmt::endl,
		"Decimal:        ", fmt::decimal(42), fmt::endl,
		"Octal:          ", fmt::octal(42), fmt::endl,
		"Hexadecimal:    ", fmt::hexadecimal(42), fmt::endl,
		"Binary:         ", fmt::binary(42), fmt::endl,
		"23-Based:       ", fmt::number<23>(42), fmt::endl,
		"Unicode:        ", fmt::unicode(U'a'), fmt::endl,
		"Align-left:     ", fmt::left(10, "Text"), fmt::endl,
		"Align-right:    ", fmt::right(10, "Text"), fmt::endl,
		"Align-center:   ", fmt::center(10, "Text"), fmt::endl,
		"Pair:           ", std::pair(fmt::decimal(42), "Text"), fmt::endl,
		"Tuple:          ", std::tuple('a', "String", fmt::unicode(U'x')), fmt::endl,
		"Plain Array:    ", fmt::apply<fmt::num_base<int>>(arr), fmt::endl,
		"Literal-String: ", fmt::literal("String1\nString2"), fmt::endl,
		"Coloured text:  ", mkAnsi(fmt::Red_BRI + fmt::Intense, "Error"),
		',', mkAnsi(fmt::Blue_BRI, "Infomation"), fmt::endl);
	return 0;
}
