#pragma once

#define text_error mkAnsi(lava::format::ErrorColour, "错误")
#define text_panic mkAnsi(lava::format::ErrorColour, "致命错误")
#define text_colon "："
#define text_quote(...) "“", __VA_ARGS__, "”"

#define msg_error text_error, text_colon
#define msg_should_not_move_to_this "不应该把一个资源对象移动到自己。"
#define msg_error_msg(err, file, line, func, msg) \
	err, text_quote(file), ':', line, ':', mkAnsi(lava::format::Yellow, func), text_colon, msg
#define msg_panic text_panic, text_colon
#define msg_unreachable_code_reached "执行到一处不可达代码："
#define msg_assert_type_names "先置条件", "后置条件", "不变式"
#define msg_condition_not_satisfied "未满足。", lava::format::endl, mkAnsi(lava::format::InfoColour, "错误信息：")
