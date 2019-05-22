#pragma once
#include <lava/config/localization.h>

namespace lava
{
	template<typename T, size_t alias = 0>
	struct tag
	{};

	// a type list
	template<typename... Ts>
	struct type_list
	{};

	// construct a list (from head and tail)
	template<typename T, typename L>
	struct cons_impl;
	template<typename T, typename... Ts>
	struct cons_impl<T, type_list<Ts...>>
	{
		using type = type_list<T, Ts...>;
	};
	template<typename T, typename L>
	using cons = typename cons_impl<T, L>::type;

	// concatenate 2 lists
	template<typename L1, typename L2>
	struct concat_impl;
	template<typename... Ts, typename... Us>
	struct concat_impl<type_list<Ts...>, type_list<Us...>>
	{
		using type = type_list<Ts..., Us...>;
	};
	template<typename L1, typename L2>
	using concat = typename concat_impl<L1, L2>::type;
} // namespace lava
