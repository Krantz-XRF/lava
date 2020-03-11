#pragma once

#define text_error mkAnsi(lava::format::legacy::ErrorColour, "error")
#define text_panic mkAnsi(lava::format::legacy::ErrorColour, "panic")
#define text_colon ": "
#define text_quote(...) '"', __VA_ARGS__, '"'

#define msg_error text_error, text_colon
#define msg_should_not_move_to_this "Should not move a resource object to itself."
#define msg_error_msg(err, file, line, func, msg) \
	err, text_quote(file), ':', line, ':', mkAnsi(lava::format::legacy::Yellow, func), text_colon, msg
#define msg_panic text_panic, text_colon
#define msg_unreachable_code_reached "unreachable codes are reached: "
#define msg_assert_type_names "pre-condition", "post-condition", "invariant"
#define msg_condition_not_satisfied " is not satisfied.", lava::format::legacy::endl, mkAnsi(lava::format::legacy::InfoColour, "message: ")

#define msg_invalid_enum_value(x, type)                                                                                 \
	"invalid enum value ", text_quote(lava::format::legacy::hexadecimal(static_cast<std::underlying_type_t<type>>(x))), \
		" of type ", text_quote(lava::trace::get_type<type>()), "."
