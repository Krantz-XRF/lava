# lava: some useful facilities for C++

## lava.format

### Introduction

`lava.format` provides a consistent interface for text formating to strings/ostreams, and can be easily extended.

3 `format_*` functions are provided: `format`, `format_s`, `format_io`:

- `format_s` takes a `std::string&`, and append formatted text to it.
- `format_io` takes a `std::ostream&`, prints formatted text to it.
- `format` is a wrapper on `format_s`, passing an empty string to `format_s` and returning the result.

In the following sections, when refering to types, the cv-qualifiers are insignificant, for all types are `std::decay`ed before they are passed to `lava::format::legacy::format_trait`.

### Getting started

Try out the code below:

```c++
namespace fmt = lava::format::legacy;
// print format result to std::ostream (e.g. std::cout)
fmt::format_io(std::cout, "String", 'c', fmt::decimal(42), fmt::endl);
// append format result to std::string
std::string data = "Some data here.";
fmt::format_s(data, "String", 'c', fmt::decimal(42), fmt::endl);
// get format result as std::string returned
std::string res = fmt::format("String", 'c', fmt::decimal(42), fmt::endl);
```

Just list the objects to format as parameters to `format_*` functions, and we will get the format result. The entities below are natively supported, and we can extend the ability of `format_*` to custom objects as describe in [Extensions](#Extensions).

### Texts

Characters and strings (`std::string` or C-style NUL-terminated strings) are formatted verbatim.

Supported types: `char`, `char*`, `std::string`.

Unicode characters are supported in a different manner. Use the `unicode` wrapper.
Unicode characters will be formatted as `U+XXXX[D]`, where `XXXX` is hexadecimal, and `D` is decimal.

Supported type: `char32_t`.

See `lava/format/text.h` for implementation details.

### Integers

Integers can be formatted in required base. They should be wrapped using the following wrappers:

- `lava::format::legacy::number<base, capital, T>`
- `lava::format::legacy::decimal<capital, T>`
- `lava::format::legacy::binary<capital, T>`
- `lava::format::legacy::octal<capital, T>`
- `lava::format::legacy::hexadecimal<capital, T>`

Explanations for template parameters:

- `capital: bool`: If latin letters are used to represent the number, whether to use capital letters. Default: `true`.
- `T: typename`: The type of the integer. The compiler should be able to deduce from the parameter.
- `base: bool`: The base for that integer. Default: 10.

Supported types: all `T` for which `std::is_integral_v<T>` is `true`.

See `lava/format/integers.h` for implementation details.

### Meta

All these entities can be formatted with an alignment and a padding character. Use the `fill` wrapper.

```c++
lava::format::legacy::fill(lava::format::legacy::ALIGN(WIDTH, [FILL]), CONTENTS...)
```

Explanations:

- ALIGN: alignment, one of `left`, `right`, `center`.
- WIDTH: the width for these entities.
- FILL: (optional) the padding character.
- CONTENTS: several entities to be formatted.

See `lava/format/meta.h` for implementation details.

### Containers

TODO

### Extensions

You may extend the ability of `format_*` functions to some custom type `T` by specializing `format_trait<T>`. Type `T` should be such a type as if it is `std::decay`ed: no top level cv-qualifiers, no reference, arrays should be expressed as pointers.

`lava.format` provides some helper macros. See the definition of `lava::format::legacy::endl` for example:

```c++
// tag `endl` will introduce a LF to the format target
SINGLETON_TAG(endl);
FORMAT_TRAIT_TAG_BEGIN(endl)
    FORMAT_STRING(endl_t) { return "\n"; }
    FORMAT_STREAM(std::ostream & os, endl_t) { std::endl(os); }
    FORMAT_APPEND(std::string & res, endl_t) { res.push_back('\n'); }
FORMAT_TRAIT_TAG_END()
```

See `lava/format/basic.h` for implementation details.

### Example for `lava.format`

This example (test case) can be found in `test/format.cpp`.

```c++
#include <iostream>
#include <lava/format/format.h>

int main()
{
    namespace fmt = lava::format::legacy;
    int arr[] = {42, 0, 1};
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
        "Align-center: ", fmt::fill(fmt::center(10), "Text"), fmt::endl,
        "Pair:         ", std::pair(fmt::decimal(42), "Text"), fmt::endl,
        "Tuple:        ", std::tuple('a', "String", fmt::unicode(U'x')), fmt::endl,
        "Plain Array:  ", fmt::apply<fmt::num_base<int>>(arr), fmt::endl);
    return 0;
}
```

Output:

```text
String:         A String
Character:      a
Decimal:        42
Octal:          52
Hexadecimal:    2A
Binary:         101010
23-Based:       1J
Unicode:        U+0061[97]
Align-left:     Text
Align-right:          Text
Align-center:      Text
Pair:           <42,Text>
Tuple:          <a,String,U+0078[120]>
Plain Array:    {42,0,1}
Literal-String: "String1\nString2"
```

Note that there is trailing spaces in lines "align-left" and "align-center". So with LF characters written out explicitly as `<LF>`, we should see

```text
Align-left:   Text      <LF>
Align-right:        Text<LF>
Align-center:    Text   <LF>
```

## lava.assert

### Explanation

TODO

See `lava/assert.h` for implementation details.

### Example for `lava.assert`

```c++
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
```

### Output

Here we print the result with `LANGUAGE` defined as `ENGLISH`.

```text
error: "assert.cpp":17:main: pre-condition[1 == 2] is not satisfied. messageThe algorithm expect 1 == 2 here.
error: "assert.cpp":18:main: post-condition[1 == 2] is not satisfied. messageThe algorithm ensures 1 == 2 here.
error: "assert.cpp":19:main: invariant[1 == 2] is not satisfied. messageThe invariant 1 == 2 should hold here.
error: "assert.cpp":20:main: unreachable codes are reachedThis code should not be reached.
error: "assert.cpp":21:main: panic: Don't panic. --The Hitchhiker's Guide to the Galaxy
```

Without defining `LANGUAGE` to `ENGLISH`, it will default to `CHINESE_SIMPLIFIED`, so the output would be like:

```text
错误: "assert.cpp":16:main: 先置条件[1 == 2]未满足。错误信息：The algorithm expect 1 == 2 here.
错误: "assert.cpp":17:main: 后置条件[1 == 2]未满足。错误信息：The algorithm ensures 1 == 2 here.
错误: "assert.cpp":18:main: 不变式[1 == 2]未满足。错误信息：The invariant 1 == 2 should hold here.
错误: "assert.cpp":19:main: 执行到一处不可达代码：This code should not be reached.
错误: "assert.cpp":20:main: 致命错误: Don't panic. --The Hitchhiker's Guide to the Galaxy
```

## lava.resource

### Explanation

TODO

See `lava/resource.h` and `lava/resource/*` for implementation details.

### Example for `lava.resource`

```c++
#include <iostream>

#include <lava/format.h>
#include <lava/resource.h>

class test : public lava::resource<test, lava::some<int*, 3>, void*, lava::many<double*>>
{
public:
    // the default constructor IS generated by default
    // define a custom constructor to suppress this, or manually delete it
    // test() = default;

    // finalizers: no custom destructor needed, all handled by the base class
    // when necessary, we should test whether the pointer passed in is nullptr
    // for lava.resource does no check on this, we should make it safe to destroy nullptrs
    void destroy(int* x) noexcept
    {
        lava::format::legacy::format_io(std::cout, "Destructing a `int*`.", lava::format::legacy::endl);
    }
    void destroy(void* x) noexcept
    {
        lava::format::legacy::format_io(std::cout, "Destructing a `void*`.", lava::format::legacy::endl);
    }
    void destroy(double* x) noexcept
    {
        lava::format::legacy::format_io(std::cout, "Destructing a `double*`.", lava::format::legacy::endl);
    }
    // define getter functions
    // these functions return references, so we can modify the value
    // the const versions return values instead
    DEFINE_GETTER_N(int*, FirstInt, 0)
    DEFINE_GETTER_N(int*, SecondInt, 1)
    DEFINE_GETTER_N(int*, ThirdInt, 2)
    DEFINE_GETTER(void*, VoidPointer)
    DEFINE_GETTER_MANY(double*, Double)
};

int main()
{
    test t;
    // move from self errors are detected using an assertion
    try
    {
        t = std::move(t);
    }
    catch (std::runtime_error& e)
    {
        lava::format::legacy::format_io(std::cerr, "Error: ", e.what(), lava::format::legacy::endl);
    }
    // use getter functions defined in class using DEFINE_GETTER_* macros
    // all these functions return resources by reference
    // while the const version returns value instead
    // t.getDouble(0);
    // the above line should raise a out-of-range error, for the vector is not initialized at all
    t.getDoubles();     // returns the std::vector<double*>
    t.getFirstInt();    // returns the first int*
    t.getVoidPointer(); // returns the void*
    return 0;
}
```

### Output

Output with `LANGUAGE` defined as `ENGLISH`:

```text
Error: error: "single.h":26:operator =: pre-condition[&obj != this] is not satisfied. message: Should not move a resource object to itself.
Destructing a `void*`.
Destructing a `int*`.
Destructing a `int*`.
Destructing a `int*`.
```

Output in Chinese Simplified (default case):

```text
Error: 错误: "single.h":26:operator =: 先置条件[&obj != this]未满足。错误信息：不应该 把一个资源对象移动到自己。
Destructing a `void*`.
Destructing a `int*`.
Destructing a `int*`.
Destructing a `int*`.
```

## lava.config

`lava.config` provides support for localization via `lava/config/localization.h` and `lava/config/language.h`.

Currently only Chinese Simplified (`CHINESE_SIMPLIFIED`, default) and English (`ENGLISH`) is supported. To use a specific language, `#define LANGUAGE <lang>` before all `lava` headers.

In future, the language options may be added to CMake configurations.
