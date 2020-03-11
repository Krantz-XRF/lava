#pragma once
#include "basic.h"
#include <iterator>
#include <tuple>

namespace lava::format::legacy
{
	template<typename T, typename U> // format a std::pair
	struct format_trait<std::pair<T, U>>
	{
		static void format_append(std::string& res, const std::pair<T, U>& p)
		{
			format_s(res, '<', p.first, ',', p.second, '>');
		}
	};

	template<typename... Args> // format a std::tuple
	struct format_trait<std::tuple<Args...>>
	{
		// iterate over a tuple
		template<typename F, typename indices, typename... Ts>
		struct tuple_f;
		template<typename F, typename... Ts, size_t... indices>
		struct tuple_f<F, std::index_sequence<indices...>, Ts...>
		{
			static void foreach_tuple(const std::tuple<Ts...>& t, F f)
			{
				(f(std::get<indices + 1>(t)), ...);
			}
		};

		// std::tuple iterate: wrapper function
		template<typename F, typename... Ts>
		static void foreach_tuple(const std::tuple<Ts...>& t, F f)
		{
			tuple_f<F, std::make_index_sequence<sizeof...(Ts) - 1>, Ts...>::foreach_tuple(t, f);
		}

		static void format_append(std::string& res, const std::tuple<Args...>& t)
		{
			format_s(res, '<', std::get<0>(t));
			foreach_tuple(t, [&res](const auto& v) { format_s(res, ',', v); });
			format_s(res, '>');
		}
	};

	template<typename F>
	struct identity
	{
		const F& f;
		identity(const F& f)
			: f{f}
		{}
	};

	template<typename F> // the identity wrapper
	struct format_trait<identity<F>>
	{
		static void format_append(std::string& res, const identity<F>& c) { format_s(res, c.f); }
	};

	template<typename F, typename C>
	struct container
	{
		const C& c;
	};
	template<typename F, typename C>
	container<F, C> apply(const C& c)
	{
		return container<F, C>{c};
	}

	template<typename F, typename Container> // format a container
	struct format_trait<container<F, Container>>
	{
		static void format_append(std::string& res, const container<F, Container>& c)
		{
			auto p = std::cbegin(c.c);
			auto pend = std::cend(c.c);
			format_s(res, '{');
			if (p != pend)
			{
				format_s(res, F{*p});
				for (++p; p != pend; ++p)
					format_s(res, ',', F{*p});
			}
			format_s(res, '}');
		}
	};
} // namespace lava::format::legacy
