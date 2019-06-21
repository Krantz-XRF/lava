#pragma once
#include <lava/assert.h>
#include <lava/resource/utility.h>

namespace lava
{
	// a single resource object
	template<typename ObjType, typename ResPtr, size_t alias = 0>
	class resource_single
	{
	public:
		using tag_t = tag<ResPtr, alias>;

		resource_single() = default;
		// initialize resource pointers
		constexpr resource_single(ResPtr ptr)
			: rawPtr{ptr}
		{}
		resource_single(resource_single&& obj) noexcept
			: rawPtr{obj.rawPtr}
		{
			obj.rawPtr = nullptr;
		}
		resource_single& operator=(resource_single&& obj) assert_except
		{
			expects(&obj != this, msg_should_not_move_to_this);
			rawPtr = obj.rawPtr;
			obj.rawPtr = nullptr;
			return *this;
		}
		// call the Destroy function from derived class
		~resource_single()
		{
			static_cast<ObjType*>(this)->destroy(rawPtr);
			rawPtr = nullptr;
		}

		// resources should not be copied
		resource_single(const resource_single&) = delete;
		resource_single& operator=(const resource_single&) = delete;
		// get resource pointers
		ResPtr getRawPtr(tag_t = {}) const noexcept { return rawPtr; }
		ResPtr& getRawPtr(tag_t = {}) noexcept { return rawPtr; }
		bool good() const noexcept { return rawPtr != nullptr; }

	protected:
		ResPtr rawPtr{nullptr};
	};
} // namespace lava
