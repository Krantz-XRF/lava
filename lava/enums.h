#pragma once
#include <array>
#include <lava/ascii.h>
#include <lava/config/localization.h>
#include <lava/trace.h>
#include <string_view>
#include <type_traits>

#ifndef LAVA_ENUM_MIN
#	define LAVA_ENUM_MIN -128
#endif

#ifndef LAVA_ENUM_MAX
#	define LAVA_ENUM_MAX 127
#endif

// Idea from Magic-Enum
namespace lava::enums
{
	template<typename E>
	struct enum_info
	{
		using underlying_type = std::underlying_type_t<E>;
		static constexpr underlying_type min = LAVA_ENUM_MIN;
		static constexpr underlying_type max = LAVA_ENUM_MAX;
		static constexpr bool bitwise = false;
	};

	namespace detail
	{
		template<typename T, T v>
		constexpr auto get_enum_value_helper() noexcept
		{
			static_assert(std::is_enum_v<T>);
#if defined(__GNUC__) || defined(__clang__)
			return std::string_view{__PRETTY_FUNCTION__, sizeof(__PRETTY_FUNCTION__) - 1};
#elif defined(_MSC_VER)
			return std::string_view{__FUNCSIG__, sizeof(__FUNCSIG__) - 1};
#else
#	error "No support for this compiler."
#endif
		}

		constexpr std::string_view maybe_id(std::string_view sig) noexcept
		{
			for (auto i = sig.size(); i > 0; --i)
				if (auto c = sig.at(i - 1); !ascii::isalnum(c))
				{
					sig.remove_prefix(i);
					break;
				}
			if (!ascii::isalpha(sig.front()))
				return {};
			return sig;
		}

		template<typename E, E v>
		constexpr std::string_view name_of() noexcept
		{
			constexpr auto signature = get_enum_value_helper<E, v>();
#if defined(__clang__) || defined(__GNUC__)
			// "constexpr auto lava::enums::get_enum_value_helper() [T = *]"
			// "constexpr auto lava::enums::get_enum_value_helper() [with T = *]"
			constexpr size_t suffix_length = sizeof("]") - 1;
#elif defined(_MSC_VER)
			// "auto __cdecl lava::enums::get_enum_value_helper<*>(void) noexcept"
			constexpr size_t suffix_length = sizeof(">(void) noexcept") - 1;
#else
#	error "Compiler not supported."
#endif
			return maybe_id(signature.substr(0, signature.size() - suffix_length));
		}

		template<typename E, typename Idx, Idx... I>
		constexpr auto entries(std::integer_sequence<Idx, I...>) noexcept
		{
			static_assert(std::is_enum_v<E>);
			constexpr std::array<Idx, sizeof...(I)> indices{I...};
			constexpr std::array<std::string_view, sizeof...(I)> names{
				name_of<E, static_cast<E>(I)>()...};
			constexpr auto count = ((name_of<E, static_cast<E>(I)>().empty() ? 0 : 1) + ...);
			std::array<std::pair<E, std::string_view>, count> result{};
			for (int i = 0, j = 0; j < count; ++i)
				if (!names[i].empty())
				{
					result[j].first = static_cast<E>(indices[i]);
					result[j].second = names[i];
					++j;
				}
			return result;
		}
	} // namespace detail

	namespace bitwise
	{
		template<typename Idx, typename T>
		struct make_bit_seq_helper;

		template<typename Idx, size_t... I>
		struct make_bit_seq_helper<Idx, std::integer_sequence<size_t, I...>>
		{
			using type = std::integer_sequence<Idx, (static_cast<Idx>(1) << I)...>;
		};

		template<typename Idx, size_t N>
		using make_bit_seq
			= typename make_bit_seq_helper<Idx, std::make_index_sequence<N>>::type;

		template<typename E>
		constexpr auto entries
			= detail::entries<E, std::underlying_type_t<E>>(
				make_bit_seq<
					std::underlying_type_t<E>,
					sizeof(std::underlying_type_t<E>) * 8>());

		template<typename E>
		constexpr auto count = entries<E>.size();
	} // namespace bitwise

	namespace dense
	{
		template<typename Idx, Idx from, typename T>
		struct make_int_seq_helper;

		template<typename Idx, Idx from, Idx... I>
		struct make_int_seq_helper<Idx, from, std::integer_sequence<size_t, I...>>
		{
			using type = std::integer_sequence<Idx, (from + I)...>;
		};

		template<typename Idx, Idx from, Idx to>
		using make_integer_range
			= typename make_int_seq_helper<Idx, from, std::make_index_sequence<to - from>>::type;

		template<typename E>
		constexpr auto entries
			= detail::entries<E, std::underlying_type_t<E>>(
				make_integer_range<
					std::underlying_type_t<E>,
					enum_info<E>::min,
					enum_info<E>::max>());

		template<typename E>
		constexpr auto count = entries<E>.size();
	} // namespace dense

	namespace dispatch
	{
		template<typename E, bool bitwise = enum_info<E>::bitwise>
		struct enum_traits;

		template<typename E>
		struct enum_traits<E, true>
		{
			static constexpr auto& entries = bitwise::entries<E>;
			static constexpr auto& count = bitwise::count<E>;
		};

		template<typename E>
		struct enum_traits<E, false>
		{
			static constexpr auto& entries = dense::entries<E>;
			static constexpr auto& count = dense::count<E>;
		};
	} // namespace dispatch

	template<typename E>
	constexpr auto entries = dispatch::enum_traits<E>::entries;

	template<typename E>
	constexpr auto count = dispatch::enum_traits<E>::count;

	template<auto v>
	constexpr std::string_view name_of() noexcept
	{
		static_assert(
			std::is_enum_v<decltype(v)>,
			"lava::enums::name_of shall only be used on enum types.");
		return detail::name_of<decltype(v), v>();
	}

	template<typename E>
	constexpr std::string_view name_of(E v) assert_except
	{
		static_assert(
			std::is_enum_v<E>,
			"lava::enums::name_of shall only be used on enum types.");
		ptrdiff_t l = 0;
		ptrdiff_t r = entries<E>.size();
		while (l < r)
		{
			ptrdiff_t m = l + (r - l) / 2;
			if (const auto mv = entries<E>[m].first; mv == v)
				return entries<E>[m].second;
			else if (mv < v)
				l = m + 1;
			else
				r = m;
		}
		unreachable(msg_invalid_enum_value(v, E));
	}

	template<typename E, bool bitwise = enum_info<E>::bitwise>
	constexpr std::underlying_type_t<E> valid_bits() noexcept
	{
		std::underlying_type_t<E> result{};
		for (const auto& [v, name] : dispatch::enum_traits<E, bitwise>::entries)
			result |= static_cast<std::underlying_type_t<E>>(v);
		return result;
	}

#define MAKE_ENUM_BITWISE(name)                   \
	namespace lava::enums                         \
	{                                             \
		template<>                                \
		struct enum_info<name>                    \
		{                                         \
			static constexpr bool bitwise = true; \
		};                                        \
	}
#define MAKE_ENUM_DENSE(name, lbound, ubound)                     \
	namespace lava::enums                                         \
	{                                                             \
		template<>                                                \
		struct enum_info<name>                                    \
		{                                                         \
			using underlying_type = std::underlying_type_t<name>; \
			static constexpr underlying_type min = lbound;        \
			static constexpr underlying_type max = ubound;        \
			static constexpr bool bitwise = false;                \
		};                                                        \
	}
} // namespace lava::enums
