#pragma once

#define text_error mkAnsi(lava::format::ErrorColour, "error")
#define text_panic mkAnsi(lava::format::ErrorColour, "panic")
#define text_colon ": "
#define text_quote(...) '"', __VA_ARGS__, '"'

#define msg_error text_error, text_colon
#define msg_should_not_move_to_this "Should not move a resource object to itself."
#define msg_error_msg(err, file, line, func, msg) \
	err, text_quote(file), ':', line, ':', mkAnsi(lava::format::Yellow, func), text_colon, msg
#define msg_panic text_panic, text_colon
#define msg_unreachable_code_reached "unreachable codes are reached: "
#define msg_assert_type_names "pre-condition", "post-condition", "invariant"
#define msg_condition_not_satisfied " is not satisfied.", lava::format::endl, mkAnsi(lava::format::InfoColour, "message: ")
