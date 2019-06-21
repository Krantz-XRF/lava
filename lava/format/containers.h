#pragma once
#include "basic.h"
#include <iterator>
#include <tuple>

namespace lava::format
{
	template<typename T, typename U> // format a std::pair
	struct format_trait<std::pair<T, U>>
	{
		static std::string format_str(const std::pair<T, U>& p)
		{
			return format('<', p.first, ',', p.second, '>');
		}
		static void format_append(std::string& res, const std::pair<T, U>& p)
		{
			format_s(res, '<', p.first, ',', p.second, '>');
		}
		static void format_stream(std::ostream& os, const std::pair<T, U>& p)
		{
			format_io(os, '<', p.first, ',', p.second, '>');
		}
	};

	template<typename... Args> // format a std::tuple
	struct format_trait<std::tuple<Args...>>
	{
		// iterate over a tuple
		template<typename F, typename indices, typename... Args>
		struct tuple_f;
		template<typename F, typename... Args, size_t... indices>
		struct tuple_f<F, std::index_sequence<indices...>, Args...>
		{
			static void foreach_tuple(const std::tuple<Args...>& t, F f)
			{
				(f(std::get<indices + 1>(t)), ...);
			}
		};

		// std::tuple iterate: wrapper function
		template<typename F, typename... Args>
		static void foreach_tuple(const std::tuple<Args...>& t, F f)
		{
			tuple_f<F, std::make_index_sequence<sizeof...(Args) - 1>, Args...>::foreach_tuple(t, f);
		}

		static std::string format_str(const std::tuple<Args...>& t)
		{
			std::string result;
			format_append(result, t);
			return result;
		}
		static void format_append(std::string& res, const std::tuple<Args...>& t)
		{
			format_s(res, '<', std::get<0>(t));
			foreach_tuple(t, [&res](const auto& v) { format_s(res, ',', v); });
			format_s(res, '>');
		}
		static void format_stream(std::ostream& os, const std::tuple<Args...>& t)
		{
			format_io(os, '<', std::get<0>(t));
			foreach_tuple(t, [&os](const auto& v) { format_io(os, ',', v); });
			format_io(os, '>');
		}
	};

	template<typename F>
	struct identity
	{
		const F& f;
	};

	template<typename F> // the identity wrapper
	struct format_trait<identity<F>>
	{
		static std::string format_str(const identity<F>& c) { return format(c.f); }
		static void format_append(std::string& res, const identity<F>& c) { format_s(res, c.f); }
		static void format_stream(std::ostream& os, const identity<F>& c) { format_io(os, c.f); }
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
		static std::string format_str(const container<F, Container>& c)
		{
			std::string res;
			format_append(res, c);
			return res;
		}
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
		static void format_stream(std::ostream& os, const container<F, Container>& c)
		{
			auto p = std::cbegin(c.c);
			auto pend = std::cend(c.c);
			format_io(os, '{');
			if (p != pend)
			{
				format_io(os, F{*p});
				for (++p; p != pend; ++p)
					format_io(os, ',', F{*p});
			}
			format_io(os, '}');
		}
	};
} // namespace lava::format
