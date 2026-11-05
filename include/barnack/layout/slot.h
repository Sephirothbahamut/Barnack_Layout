#pragma once

#include <cmath>
#include <vector>
#include <functional>
#include <unordered_set>

#include "base_types.h"

namespace barnack::layout
	{
	class element;
	using element_ref   = std  ::reference_wrapper<element    >;
	using element_own   = std  ::unique_ptr       <element    >;
	using element_obs   = utils::observer_ptr     <element    >;
	using elements_refs = std  ::vector           <element_ref>;

	class element;

	struct slot
		{
		layout::element_own child_own;

		template <typename T, typename ...Args>
		T& emplace(Args&&... args) noexcept
			{
			std::unique_ptr<T> tmp{std::make_unique<T>(std::forward<Args>(args)...)};
			utils::observer_ptr<T> ptr{tmp.get()};
			child_own = std::move(tmp);
			return *ptr;
			}

		template <typename T>
		utils::observer_ptr<T> move_in(std::unique_ptr<element>&& new_child_own) noexcept
			{
			assert(new_child_own);
			utils::observer_ptr<T> ptr{new_child_own.get()};
			child_own = std::move(new_child_own);
			return ptr;
			}
		};

	template <typename alignment_T>
	struct slot_with_alignment : slot
		{
		using alignment_t = alignment_T;
		alignment_t alignment{utils::alignment::default_value<alignment_t>()};

		slot_with_alignment(alignment_t alignment);
		};

	namespace details
		{
		/// <summary> Used to guarantee that a child exists.</summary>
		template <std::derived_from<slot> slot_T>
		class slot_access_helper : public utils::oop::non_copyable
			{
			public:
				using slot_t = slot_T;

			private:
				utils::observer_ptr<slot_t > slot_obs  {nullptr};
				utils::observer_ptr<element> parent_obs{nullptr};
				bool valid() const noexcept;
				void nullify() noexcept;

			public:
				slot_access_helper(slot_t& slot, element& parent);
				~slot_access_helper();

				slot_access_helper           (slot_access_helper&& move);
				slot_access_helper& operator=(slot_access_helper&& move);

				template <std::derived_from<element> T, typename ...Args>
				T& emplace(Args&&... args) noexcept
					{
					assert(valid());
					auto& ret{slot_obs->emplace<T>(std::forward<Args>(args)...)};
					ret.parent_obs = parent_obs;
					return ret;
					}

				template <typename T>
				utils::observer_ptr<T> move_in(std::unique_ptr<element>&& new_child_own) noexcept
					{
					assert(valid());
					auto& ret{slot_obs->move_in<T>(std::move(new_child_own))};
					ret.parent_obs = parent_obs;
					return ret;
					}
			};
		}
	}



#ifdef barnack_layout_implementation
#include "slot.cpp"
#endif