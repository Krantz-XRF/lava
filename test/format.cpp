#include <iostream>
#include <lava/format/format.h>

int main()
{
	namespace fmt = lava::format;
	fmt::format_io(
		std::cout,
		"String:       ", "A String", fmt::endl,
		"Character:    ", 'a', fmt::endl,
		"Decimal:      ", fmt::decimal(42), fmt::endl,
		"Octal:        ", fmt::octal(42), fmt::endl,
		"Hexadecimal:  ", fmt::hexadecimal(42), fmt::endl,
		"Binary:       ", fmt::binary(42), fmt::endl,
		"23-Based:     ", fmt::number<23>(42), fmt::endl,
		"Unicode:      ", fmt::unicode(U'a'), fmt::endl,
		"Align-left:   ", fmt::fill(fmt::left(10), "Text"), fmt::endl,
		"Align-right:  ", fmt::fill(fmt::right(10), "Text"), fmt::endl,
		"Align-center: ", fmt::fill(fmt::center(10), "Text"), fmt::endl);
	return 0;
}
