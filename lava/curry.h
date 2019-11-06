#pragma once
#include <functional>
#include <map>
#include <vector>

namespace lava::curry
{
	template<typename F>
	decltype(auto) curry(F&& f);

	namespace detail
	{
		template<typename Res, typename... Args>
		struct curry_impl;

		// Type of the curried function
		template<typename Res, typename... Args>
		using curry_t = typename curry_impl<Res(Args...)>::type;

		// Get function signature of various function-like objects
		template<typename T>
		struct signature;

		// Signature for plain old function pointers
		template<typename Res, typename... Args>
		struct signature<Res (*)(Args...)>
		{
			using type = Res(Args...);
		};

		// Signature for std::function
		template<typename Res, typename... Args>
		struct signature<std::function<Res(Args...)>>
		{
			using type = Res(Args...);
		};

		// Signature for member functions
		template<typename C, typename Res, typename... Args>
		struct signature<Res (C::*)(Args...)>
		{
			using type = Res(Args...);
		};

		// Signature for const member functions
		// (with constness removed)
		template<typename C, typename Res, typename... Args>
		struct signature<Res (C::*)(Args...) const>
		{
			using type = Res(Args...);
		};

		// Base case: no parameter
		template<typename Res>
		struct curry_impl<Res()>
		{
			using type = std::function<Res()>;
			static type curry(type f) { return f; }
		};

		// Base case: 1 parameter
		template<typename Res, typename Arg>
		struct curry_impl<Res(Arg)>
		{
			using type = std::function<Res(Arg)>;
			static type curry(type f) { return f; }
		};

		// Recursive case: 2 or more parameters
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
	} // namespace detail

	template<typename T>
	using signature_t = typename detail::signature<T>::type;

	// the curry function
	template<typename F>
	decltype(auto) curry(F&& f)
	{
		return detail::curry_impl<signature_t<decltype(&F::operator())>>::curry(f);
	}
} // namespace lava::curry