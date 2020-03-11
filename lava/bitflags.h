#pragma once
#include <cstdint>
#include <initializer_list>
#include <lava/enums.h>
#include <lava/format/legacy.h>

namespace lava
{
	inline constexpr uint32_t bit(int x) { return 1u << x; }

	template<typename T>
	class bitflags
	{
	public:
		static_assert(std::is_enum_v<T>, "bitflags只应用于enum类型。");
		using underlying_type = std::underlying_type_t<T>;

		static constexpr underlying_type decay(T val) noexcept { return static_cast<underlying_type>(val); }
		static constexpr underlying_type valid_bits = enums::valid_bits<T, true>();

		// 使得bitflags是POD的
		constexpr bitflags() noexcept = default;
		constexpr bitflags(const bitflags&) noexcept = default;
		constexpr bitflags(bitflags&&) noexcept = default;
		~bitflags() noexcept = default;
		bitflags& operator=(const bitflags&) noexcept = default;
		bitflags& operator=(bitflags&&) noexcept = default;

		// 类型转换构造函数：允许隐式转换
		bitflags(std::initializer_list<T> vals) noexcept
		{
			for (auto v : vals)
				value |= v;
		}
		// 不允许相反方向的转换，标志位必须用专门的类型表示

		// 类型转换构造函数：允许从底层类型构造标志位，必须显式转换
		explicit bitflags(underlying_type val) noexcept
			: value{val}
		{}

#define CHECK_TYPES(Ts) static_assert((std::is_same_v<Ts, T> && ...), "只能指定枚举类型T的标志位。")
		// 检验标志位非空
		bool test() const noexcept { return value != 0; }
		explicit operator bool() const noexcept { return test(); }
		bool operator!() const noexcept { return !test(); }
		// 检验设置了特定的标志位
		bool test(T flag) const noexcept { return (value & decay(flag)) == decay(flag); }
		// 检验同时设置了一系列的标志位
		template<typename... Ts>
		bool all(Ts... flags) const noexcept
		{
			CHECK_TYPES(Ts);
			return (test(flags) && ...);
		}
		// 检验设置了一系列的标志位中的某一个
		template<typename... Ts>
		bool any(Ts... flags) const noexcept
		{
			CHECK_TYPES(Ts);
			return (test(flags) || ...);
		}
		// 设置一个标志位
		void set(T flag) { value |= decay(flag); }
		// 取消设置一个标志位
		void unset(T flag) { value &= ~decay(flag); }
		// 设置一系列标志位
		template<typename... Ts>
		void set(Ts... flags)
		{
			CHECK_TYPES(Ts);
			(set(flags), ...);
		}
		// 取消设置一系列标志位
		template<typename... Ts>
		void unset(Ts... flags)
		{
			CHECK_TYPES(Ts);
			(unset(flags), ...);
		}
#undef CHECK_TYPES

		// 退化成底层类型
		underlying_type decay() const noexcept { return value; }

	private:
		underlying_type value{0};
	};

	namespace format::legacy
	{
		template<typename T>
		struct format_trait<bitflags<T>>
		{
			static void format_append(std::string& res, bitflags<T> flags)
			{
				auto x = flags.decay();
				res.append("[");
				if (x != 0)
				{
					auto bit = static_cast<T>(x & -x);
					res.append(enums::name_of(bit));
					x &= x - 1;
				}
				while (x != 0)
				{
					auto bit = static_cast<T>(x & -x);
					res.append(", ");
					res.append(enums::name_of(bit));
					x &= x - 1;
				}
				res.append("]");
			}
		};
	} // namespace format::legacy

	template<typename T, typename... Ts>
	bitflags<T> make_flags(T flag, Ts... flags)
	{
		static_assert((std::is_same_v<Ts, T> && ...), "所有给定的标志位必须是同样的枚举类型。");
		return bitflags<T>((flag | ... | flags));
	}

#define DEFINE_BITFLAGS_BINARY_OPERATOR(op)                   \
	template<typename T>                                      \
	bitflags<T> operator op(bitflags<T> lhs, bitflags<T> rhs) \
	{                                                         \
		return bitflags<T>(lhs.decay() op rhs.decay());       \
	}
#define DEFINE_BITFLAGS_UNARY_OPERATOR(op)     \
	template<typename T>                       \
	bitflags<T> operator op(bitflags<T> flags) \
	{                                          \
		return bitflags<T>(op flags.decay());  \
	}

	// 允许对标志位的交集操作
	DEFINE_BITFLAGS_BINARY_OPERATOR(&)
	// 允许对标志位的并集操作
	DEFINE_BITFLAGS_BINARY_OPERATOR(|)
	// 允许对标志位的对称差操作
	DEFINE_BITFLAGS_BINARY_OPERATOR(^)
	// 允许对标志位的补集操作
	template<typename T>
	bitflags<T> operator~(bitflags<T> flags)
	{
		return bitflags<T>(bitflags<T>::valid_bits & ~flags.decay());
	}

#undef DEFINE_BITFLAGS_BINARY_OPERATOR
#undef DEFINE_BITFLAGS_UNARY_OPERATOR

#define DEFINE_BITFLAGS_ASSIGNMENT(op)                             \
	template<typename T>                                           \
	bitflags<T>& operator op##=(bitflags<T>& lhs, bitflags<T> rhs) \
	{                                                              \
		return lhs = (lhs op rhs);                                 \
	}

	// 赋值：对标志位的交集操作
	DEFINE_BITFLAGS_ASSIGNMENT(&)
	// 赋值：对标志位的并集操作
	DEFINE_BITFLAGS_ASSIGNMENT(|)
	// 赋值：对标志位的对称差操作
	DEFINE_BITFLAGS_ASSIGNMENT(^)

#undef DEFINE_BITFLAGS_ASSIGNMENT

	template<typename T>
	bool operator==(bitflags<T> lhs, bitflags<T> rhs)
	{
		return lhs.decay() == rhs.decay();
	}

	template<typename T>
	bool operator!=(bitflags<T> lhs, bitflags<T> rhs)
	{
		return lhs.decay() != rhs.decay();
	}
} // namespace lava
