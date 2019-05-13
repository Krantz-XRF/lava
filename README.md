# lava: some useful facilities for C++

## lava.format

`lava.format` provides a consistent interface for text formating to strings/ostreams, and can be easily extended.

3 `format_*` functions are provided: `format`, `format_s`, `format_io`:

- `format_s` takes a `std::string&`, and append formatted text to it.
- `format_io` takes a `std::ostream&`, prints formatted text to it.
- `format` is a wrapper on `format_s`, passing an empty string to `format_s` and returning the result.

In the following sections, when refering to types, the cv-qualifiers are insignificant, for all types are `std::decay`ed before they are passed to `lava::format::format_trait`.

The entities below are natively supported.

### Texts

Characters and strings (`std::string` or C-style NUL-terminated strings) are formatted verbatim.

Supported types: `char`, `char*`, `std::string`.

Unicode characters are supported in a different manner. Use the `unicode` wrapper.
Unicode characters will be formatted as `U+XXXX[D]`, where `XXXX` is hexadecimal, and `D` is decimal.

Supported type: `char32_t`.

See `lava/format/text.h` for implementation details.

### Integers

Integers can be formatted in required base. They should be wrapped using the following wrappers:

- `lava::format::number<base, capital, T>`
- `lava::format::decimal<capital, T>`
- `lava::format::binary<capital, T>`
- `lava::format::octal<capital, T>`
- `lava::format::hexadecimal<capital, T>`

Explanations for template parameters:

- `capital: bool`: If latin letters are used to represent the number, whether to use capital letters. Default: `true`.
- `T: typename`: The type of the integer. The compiler should be able to deduce from the parameter.
- `base: bool`: The base for that integer. Default: 10.

Supported types: all `T` for which `std::is_integral_v<T>` is `true`.

See `lava/format/integers.h` for implementation details.

### Meta

All these entities can be formatted with an alignment and a padding character. Use the `fill` wrapper.

```c++
lava::format::fill(lava::format::ALIGN(WIDTH, [FILL]), CONTENTS...)
```

Explanations:

- ALIGN: alignment, one of `left`, `right`, `center`.
- WIDTH: the width for these entities.
- FILL: (optional) the padding character.
- CONTENTS: several entities to be formatted.

See `lava/format/meta.h` for implementation details.

### Extensions

You may extend the ability of `format_*` functions to some custom type `T` by specializing `format_trait<T>`. Type `T` should be such a type as if it is `std::decay`ed: no top level cv-qualifiers, no reference, arrays should be expressed as pointers.

`lava.format` provides some helper macros. See the definition of `lava::format::endl` for example:

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
    namespace fmt = lava::format;
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
        "Align-center: ", fmt::fill(fmt::center(10), "Text"), fmt::endl);
    return 0;
}
```

## lava.assert

### Explanation

TODO

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
