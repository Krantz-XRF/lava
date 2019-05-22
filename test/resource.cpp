#include <iostream>

#include <lava/format.h>
#include <lava/resource.h>

class test : public lava::resource<test, lava::some<int*, 3>, void*, lava::many<double*>>
{
public:
	test() = default;
	void Destroy(int* x) noexcept
	{
		lava::format::format_io(std::cout, "Destructing a `int*`.", lava::format::endl);
	}
	void Destroy(void* x) noexcept
	{
		lava::format::format_io(std::cout, "Destructing a `void*`.", lava::format::endl);
	}
	void Destroy(double* x) noexcept
	{
		lava::format::format_io(std::cout, "Destructing a `double*`.", lava::format::endl);
	}
};

int main()
{
	test t;
	return 0;
}
