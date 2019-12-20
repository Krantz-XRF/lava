#include <iostream>
#include <lava/finally.h>

void test_block(bool should_throw)
{
	auto _final = lava::finally([] { std::cout << "lava::finally();\n"; });
	auto _success = lava::on_success([] { std::cout << "lava::on_success();\n"; });
	auto _failure = lava::on_exception([] { std::cout << "lava::on_exception();\n"; });
	if (should_throw)
		throw 0;
}

void test(bool should_throw)
{
	try
	{
		test_block(should_throw);
	}
	catch (int)
	{}
}

int main()
{
	std::cout << "When no exception is thrown:\n";
	test(false);
	std::cout << "When an int is thrown:\n";
	test(true);
	return 0;
}
