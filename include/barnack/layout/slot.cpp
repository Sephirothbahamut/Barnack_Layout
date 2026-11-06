#include "slot.h"

#include "element.h"

namespace barnack::layout
	{
	template <typename alignment_t>
	slot_with_alignment<alignment_t>::slot_with_alignment() : alignment{utils::alignment::default_value<alignment_t>()} {}

	template <typename alignment_t>
	slot_with_alignment<alignment_t>::slot_with_alignment(alignment_t alignment) : alignment{alignment} {}

	namespace details
		{
		template <std::derived_from<slot> slot_t>
		bool slot_access_helper<slot_t>::valid() const noexcept
			{
			return (slot_obs != nullptr) && (parent_obs != nullptr);
			}

		template <std::derived_from<slot> slot_t>
		void slot_access_helper<slot_t>::nullify() noexcept
			{
			slot_obs   = nullptr;
			parent_obs = nullptr;
			}

		template <std::derived_from<slot> slot_t>
		slot_access_helper<slot_t>::slot_access_helper(slot_access_helper&& move) : 
			slot_obs  {std::move(move.slot_obs  )}, 
			parent_obs{std::move(move.parent_obs)}
			{
			nullify();
			}

		template <std::derived_from<slot> slot_t>
		slot_access_helper<slot_t>& slot_access_helper<slot_t>::operator=(slot_access_helper&& move)
			{
			slot_obs   = std::move(move.slot_obs  ); 
			parent_obs = std::move(move.parent_obs);
			nullify();
			return *this;
			}

		template <std::derived_from<slot> slot_t>
		slot_access_helper<slot_t>::slot_access_helper(slot_t& slot, element& parent) : 
			slot_obs  {std::addressof(slot  )}, 
			parent_obs{std::addressof(parent)}
			{}

		template <std::derived_from<slot> slot_t>
		slot_access_helper<slot_t>::~slot_access_helper()
			{
			if(valid() && (slot_obs->child_own == nullptr))
				{
				assert((slot_obs->child_own == nullptr) && "Slots must not be empty. If you create a slot you must emplace an element inside it.");
				}
			}
		}

	template struct slot_with_alignment<utils::alignment            >;
	template struct slot_with_alignment<utils::alignment::vertical  >;
	template struct slot_with_alignment<utils::alignment::horizontal>;

	namespace details
		{
		template class slot_access_helper<layout::slot                                             >;
		template class slot_access_helper<layout::slot_with_alignment<utils::alignment            >>;
		template class slot_access_helper<layout::slot_with_alignment<utils::alignment::vertical  >>;
		template class slot_access_helper<layout::slot_with_alignment<utils::alignment::horizontal>>;
		}
	}

