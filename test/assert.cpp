#include <iostream>
#include <lava/assert.h>

#define test_assert(assert)                 \
	try                                     \
	{                                       \
		assert;                             \
	}                                       \
	catch (std::runtime_error & e)          \
	{                                       \
		std::cerr << e.what() << std::endl; \
	}

int main()
{
	test_assert(expects(1 == 2, "The algorithm expect 1 == 2 here."));
	test_assert(ensures(1 == 2, "The algorithm ensures 1 == 2 here."));
	test_assert(invariant(1 == 2, "The invariant 1 == 2 should hold here."));
	test_assert(unreachable("This code should not be reached."));
	test_assert(panic("Don't panic. --The Hitchhiker's Guide to the Galaxy"));
	return 0;
}
