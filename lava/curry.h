#pragma once
#include <functional>
#include <map>
#include <vector>

namespace lava::curry
{
	template<typename Res, typename... Args>
	struct curry_impl;

	template<typename Res, typename... Args>
	using curry_t = typename curry_impl<Res(Args...)>::type;

	template<typename Res, typename... Args>
	curry_t<Res, Args...> curry(std::function<Res(Args...)> f)
	{
		return curry_impl<Res(Args...)>::curry(f);
	}

	template<typename Res, typename... Args>
	decltype(auto) curry(Res (*f)(Args...))
	{
		return curry(std::function<Res(Args...)>(f));
	}

	template <typename T>
	struct lambda_to_function;

	template <typename C,typename Res, typename ...Args>
	struct lambda_to_function<Res(C::*)(Args...)>
	{
		using type = Res(Args...);
	};

	template <typename C,typename Res, typename ...Args>
	struct lambda_to_function<Res(C::*)(Args...) const>
	{
		using type = Res(Args...);
	};

	template <typename T>
	using lambda_to_function_t = typename lambda_to_function<T>::type;

	template<typename F>
	decltype(auto) curry(F&& f)
	{
		return curry_impl<lambda_to_function_t<decltype(&F::operator())>>::curry(f);
	}

	template<typename Res>
	struct curry_impl<Res()>
	{
		using type = std::function<Res()>;
		static type curry(type f) { return f; }
	};

	template<typename Res, typename Arg>
	struct curry_impl<Res(Arg)>
	{
		using type = std::function<Res(Arg)>;
		static type curry(type f) { return f; }
	};

	template<typename Res, typename Arg, typename... Args>
	struct curry_impl<Res(Arg, Args...)>
	{
		using type = std::function<curry_t<Res, Args...>(Arg)>;
		static type curry(std::function<Res(Arg, Args...)> f)
		{
			return [f = std::move(f)](Arg x) -> curry_t<Res, Args...>
			{
				return lava::curry::curry(std::function<Res(Args...)>(
					[x = std::move(x), f = std::move(f)](Args... args) {
						return f(x, args...);
					}));
			};
		}
	};
} // namespace lava::curry