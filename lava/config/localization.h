#pragma once
#include <lava/config/language.h>

#if LANGUAGE == CHINESE_SIMPLIFIED
#	include <lava/config/zh-cn.h>
#elif LANGUAGE == ENGLISH
#	include <lava/config/en.h>
#else
#	error "Unsupported language! Only zh_CN and en is supported."
#endif
