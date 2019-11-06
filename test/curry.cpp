#include <iostream>
#include <lava/curry.h>
#include <lava/trace.h>

namespace fmt = lava::format;

void lava::trace::trace_message(const std::string& msg)
{
	std::cout << msg;
}

int main()
{
	auto f = lava::curry::curry([](int x, int y) { return x + y; });
	traceShow(fmt::decimal, f(4)(2));
	return 0;
}