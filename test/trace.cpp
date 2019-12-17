#include <iostream>
#include <lava/trace.h>

namespace fmt = lava::format;

setTraceOutput(std::cout);

int main(int argc, char* argv[])
{
	trace("This is a message.");
	traceShow(fmt::decimal, 6 * 7);
	return 0;
}
