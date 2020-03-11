#pragma once
#include <lava/resource/single.h>
#include <lava/resource/utility.h>
#include <utility>

namespace lava
{
	// `some` means the resource object will hold some fixed amount of the given pointer type
	template<typename ResPtr, size_t count>
	struct some
	{};

	// expand some to resource_single<>
	template<typename ObjType, typename ResPtr, typename Indices>
	struct expand_some_impl;
	template<typename ObjType, typename ResPtr, size_t... indices>
	struct expand_some_impl<ObjType, ResPtr, std::index_sequence<indices...>>
	{
		using type = type_list<resource_single<ObjType, ResPtr, indices>...>;
	};
	template<typename ObjType, typename ResPtr, size_t count>
	using expand_some = typename expand_some_impl<ObjType, ResPtr, std::make_index_sequence<count>>::type;
} // namespace lava
