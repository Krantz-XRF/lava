#pragma once
#include "config/language.h"

#if LANGUAGE == CHINESE_SIMPLIFIED
#	include "assert/assert-zh-cn.h"
#elif LANGUAGE == ENGLISH
#	include "assert/assert-en.h"
#else
#	error "Unsupported language! Only zh_CN and en is supported."
#endif
