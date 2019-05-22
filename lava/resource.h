#pragma once
#include <lava/resource/single.h>
#include <lava/resource/many.h>
#include <lava/resource/some.h>

namespace lava
{
	// expand all helper types to resource_single<>
	template<typename ObjType, typename... ResPtr>
	struct expand_types_impl;
	template<typename ObjType, typename... ResPtr>
	using expand_types = typename expand_types_impl<ObjType, ResPtr...>::type;
	template<typename ObjType>
	struct expand_types_impl<ObjType>
	{
		using type = type_list<>;
	};
	template<typename ObjType, typename ResPtr, size_t count, typename... Rs>
	struct expand_types_impl<ObjType, some<ResPtr, count>, Rs...>
	{
		using type = concat<expand_some<ObjType, ResPtr, count>, expand_types<ObjType, Rs...>>;
	};
	template<typename ObjType, typename ResPtr, template<typename> typename Container, typename... Rs>
	struct expand_types_impl<ObjType, many<ResPtr, Container>, Rs...>
	{
		using type = cons<resource_many<ObjType, ResPtr, 0>, expand_types<ObjType, Rs...>>;
	};
	template<typename ObjType, typename ResPtr, typename... Rs>
	struct expand_types_impl<ObjType, ResPtr, Rs...>
	{
		using type = cons<resource_single<ObjType, ResPtr, 0>, expand_types<ObjType, Rs...>>;
	};

	template<typename ObjType, typename Objs>
	class resource_impl;

	template<typename ObjType, typename... BaseObjs>
	class resource_impl<ObjType, type_list<BaseObjs...>> : public BaseObjs...
	{
	public:
		resource_impl() = default;
		resource_impl(resource_impl&&) = default;
		resource_impl(const resource_impl&) = delete;
		~resource_impl() = default;

		resource_impl& operator=(resource_impl&& obj)
		{
			(BaseObjs::operator=(std::move(obj)), ...);
			return *this;
		}
		resource_impl& operator=(const resource_impl&) = delete;

		bool good() const noexcept { return (BaseObjs::good() && ...); }

	protected:
		using BaseObjs::getRawPtr...;
	};

	template<typename ObjType, typename... ResPtrs>
	using resource = resource_impl<ObjType, expand_types<ObjType, ResPtrs...>>;
} // namespace lava

#define DECLARE_OBJECT(type)                    \
	type(type&&) noexcept = default;            \
	type(const type&) = delete;                 \
	~type() noexcept = default;                 \
	type& operator=(type&&) noexcept = default; \
	type& operator=(const type&) = delete;

#define DEFINE_GETTER(type, name)                                 \
	type& get##name() noexcept { return getRawPtr(tag<type>{}); } \
	type get##name() const noexcept { return getRawPtr(tag<type>{}); }
#define DEFINE_GETTER_N(type, name, n)                               \
	type& get##name() noexcept { return getRawPtr(tag<type, n>{}); } \
	type get##name() const noexcept { return getRawPtr(tag<type, n>{}); }
#define DEFINE_GETTER_MANY(type, name)                                                \
	decltype(auto) get##name##s() noexcept { return getRawPtr(tag<type>{}); }         \
	decltype(auto) get##name##s() const noexcept { return getRawPtr(tag<type>{}); }   \
	decltype(auto) get##name(size_t i) noexcept { return getRawPtr(tag<type>{})[i]; } \
	decltype(auto) get##name(size_t i) const noexcept { return getRawPtr(tag<type>{})[i]; }
