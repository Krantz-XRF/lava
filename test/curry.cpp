#include <iostream>
#include <lava/curry.h>
#include <lava/trace.h>

namespace fmt = lava::format;

setTraceOutput(std::cout);

int plus(int x, int y) { return x + y; }

int main()
{
	auto f = lava::curry::curry([](int x, int y) { return x + y; });
	auto g = lava::curry::curry(plus);
	traceShow(fmt::decimal, f(4)(2));
	traceShow(fmt::decimal, g(4)(2));
	return 0;
}
