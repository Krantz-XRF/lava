#include <iostream>

#include <lava/format.h>
#include <lava/resource.h>

class test : public lava::resource<test, lava::some<int*, 3>, void*, lava::many<double*>>
{
public:
	test() = default;
	DECLARE_OBJECT(test)
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
	DEFINE_GETTER_N(int*, FirstInt, 0)
	DEFINE_GETTER_N(int*, SecondInt, 1)
	DEFINE_GETTER_N(int*, ThirdInt, 2)
	DEFINE_GETTER(void*, VoidPointer)
	DEFINE_GETTER_MANY(double*, Double)
};

int main()
{
	test t;
	return 0;
}
