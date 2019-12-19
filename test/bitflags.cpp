#include <iostream>
#include <lava/assert.h>
#include <lava/bitflags.h>
#include <lava/format.h>
#include <lava/trace.h>

enum LavaLibs
{
	Config = lava::bit(0),
	Format = lava::bit(1),
	Resource = lava::bit(2),
	Assert = lava::bit(3),
	Bitflags = lava::bit(4),
	Trace = lava::bit(5),
};
MAKE_ENUM_BITWISE(LavaLibs)

setTraceOutput(std::cout);

namespace fmt = lava::format;

int main()
{
	lava::bitflags<LavaLibs> f;
	f.set(Config, Format, Assert, Bitflags);
	ensures(f.test(Bitflags));
	ensures(f.all(Assert, Config, Format));
	ensures(!f.any(Trace, Resource));
	trace(f);

	lava::bitflags<LavaLibs> g{Config, Format, Resource};
	ensures((f & g) == lava::make_flags(Config, Format));
	ensures((f | g) == lava::make_flags(Config, Format, Assert, Bitflags, Resource));
	ensures((f ^ g) == lava::make_flags(Assert, Bitflags, Resource));
	ensures((~f) == lava::make_flags(Resource, Trace));
	trace(g);

	return 0;
}
