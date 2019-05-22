#pragma once
#define msg_should_not_move_to_this "不应该把一个资源对象移动到自己。"
#define msg_error_msg(file, line, func, msg) "错误: \"", file, "\":", line, ':', func, ": ", msg
#define msg_panic "致命错误: "
#define msg_unreachable_code_reached "执行到一处不可达代码："
#define msg_assert_type_names "先置条件", "后置条件", "不变式"
#define msg_condition_not_satisfied "未满足。错误信息："
