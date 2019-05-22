#pragma once
#include <lava/assert.h>
#include <lava/resource/utility.h>
#include <vector>

namespace lava
{
	// the vector container of objects
	template<typename ObjType, typename ResPtr, size_t alias = 0, template<typename> typename Container = std::vector>
	class resource_many
	{
	public:
		using tag_t = tag<ResPtr, alias>;
		using resource_type = Container<ResPtr>;

		resource_many() = default;
		// initialize resource pointers
		constexpr resource_many(std::initializer_list<ResPtr> lst)
			: resource{lst}
		{}
		resource_many(resource_many&& obj) noexcept
			: resource{std::move(obj.resource)}
		{}
		resource_many& operator=(resource_many&& obj) assert_except
		{
			expects(&obj != this, msg_should_not_move_to_this);
			resource = std::move(obj.resource);
			return *this;
		}
		// call the Destroy function from derived class
		~resource_many()
		{
			for (auto rawPtr : resource)
				static_cast<ObjType*>(this)->Destroy(rawPtr);
			resource.clear();
		}

		// resources should not be copied
		resource_many(const resource_many&) = delete;
		resource_many& operator=(const resource_many&) = delete;
		// get resource pointers
		const resource_type& getRawPtr(tag_t = {}) const noexcept { return resource; }
		resource_type& getRawPtr(tag_t = {}) noexcept { return resource; }
		bool good() const noexcept
		{
			for (auto rawPtr : resource)
				if (rawPtr == nullptr)
					return false;
			return true;
		}

	protected:
		resource_type resource;
	};

	// `many` means the resource object will hold a dynamic container of the given pointer type
	template<typename ResPtr, template<typename> typename Container = std::vector>
	struct many
	{};
} // namespace lava
