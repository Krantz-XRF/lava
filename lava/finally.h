#pragma once
#include <exception>

namespace lava
{
	namespace detail
	{
		enum class exec_strategy
		{
			successful,
			exceptional,
			always,
		};

		template<typename F, exec_strategy strat>
		class exec_finally
		{
		public:
			exec_finally(F func) noexcept
				: func{std::move_if_noexcept(func)}
				, original_exceptions{std::uncaught_exceptions()}
			{}
			~exec_finally() noexcept
			{
				auto current_exceptions = std::uncaught_exceptions();
				if (should_exec(original_exceptions, current_exceptions)) func();
			}

			[[nodiscard]] static constexpr bool should_exec(
				[[maybe_unused]] int original,
				[[maybe_unused]] int current) noexcept
			{
				if constexpr (strat == exec_strategy::always)
					return true;
				else if constexpr (strat == exec_strategy::successful)
					return original == current;
				else
					return original < current;
			}

		private:
			F func;
			const int original_exceptions;
		};

		template<exec_strategy strat, typename F>
		auto execute_finally(F&& func)
		{
			return exec_finally<std::decay_t<F>, strat>(std::forward<F>(func));
		}
	} // namespace detail

	template<typename F>
	auto finally(F&& func)
	{
		return detail::execute_finally<detail::exec_strategy::always>(std::forward<F>(func));
	}

	template<typename F>
	auto on_success(F&& func)
	{
		return detail::execute_finally<detail::exec_strategy::successful>(std::forward<F>(func));
	}

	template<typename F>
	auto on_exception(F&& func)
	{
		return detail::execute_finally<detail::exec_strategy::exceptional>(std::forward<F>(func));
	}
} // namespace lava
