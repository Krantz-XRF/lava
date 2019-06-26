#include <iostream>
#include <lava/trace.h>

namespace fmt = lava::format;

void lava::trace::trace_message(const std::string& msg)
{
	std::cout << msg;
}

int main(int argc, char* argv[])
{
	trace("This is a message.");
	traceShow(fmt::decimal, 5);
	return 0;
}
