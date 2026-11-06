#include "common.h"

namespace barnack::layout::wrapper::details
	{
	template <typename slot_t>
	bool with_slot_t<slot_t>::visit(const std::function<bool(element& element)>& callback) noexcept
		{
		const bool ret{element::visit(callback)};
		if (ret)
			{
			child().visit(callback);
			}
		return ret;
		}

	template <typename slot_t>
	[[nodiscard]] layout::details::slot_access_helper<slot_t> with_slot_t<slot_t>::get_slot() noexcept
		{
		return layout::details::slot_access_helper<slot_t>{slot, *this};
		}

	template <typename slot_t>
	[[nodiscard]] auto with_slot_t<slot_t>::get_slot(const layout::element& previous_element) noexcept -> std::optional<layout::details::slot_access_helper<slot_t>>
		{
		if (slot.child_own.get() == std::addressof(previous_element))
			{
			return layout::details::slot_access_helper<slot_t>{slot, *this};
			}
		return std::nullopt;
		}

	template <typename slot_t>
	const layout::element& with_slot_t<slot_t>::child() const noexcept
		{
		assert(this->slot.child_own);
		return *slot.child_own;
		}

	template <typename slot_t>
	layout::element& with_slot_t<slot_t>::child() noexcept
		{
		assert(this->slot.child_own);
		return *slot.child_own;
		}

	template <typename slot_t>
	void with_slot_t<slot_t>::evaluate_traversal_for_children(layout::traversal& traversal, layout::traversal::node& previously_visited_my_node, const utils::math::vec2f& position) noexcept
		{
		auto& visited_children{previously_visited_my_node.children};
				
		if (!child().evaluate_traversal(traversal, previously_visited_my_node, position))
			{
			auto it{std::find_if(visited_children.begin(), visited_children.end(), [&](const layout::traversal::node& node) { return node.element_obs == std::addressof(child()); })};
			if (it != visited_children.end())
				{
				traversal._left_roots.emplace_back(std::move(*it));
				visited_children.erase(it);
				}
			}
		}
	template <typename slot_t>
	void with_slot_t<slot_t>::evaluate_traversal_for_children(layout::traversal::node& my_node, const utils::math::vec2f& position) noexcept
		{
		child().evaluate_traversal(my_node, position);
		}

	template <typename alignment_t>
	with_alignment<alignment_t>::with_alignment(alignment_t alignment) : with_slot_t<layout::slot_with_alignment<alignment_t>>{alignment}
		{}

	template class with_slot_t<layout::slot                                             >;
	template class with_slot_t<layout::slot_with_alignment<utils::alignment            >>;
	template class with_slot_t<layout::slot_with_alignment<utils::alignment::vertical  >>;
	template class with_slot_t<layout::slot_with_alignment<utils::alignment::horizontal>>;

	template class with_alignment                       <utils::alignment            > ;
	template class with_alignment                       <utils::alignment::vertical  > ;
	template class with_alignment                       <utils::alignment::horizontal> ;
	}