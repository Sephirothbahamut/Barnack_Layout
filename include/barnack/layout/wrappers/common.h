#pragma once

#include "common.h"

#include <utils/alignment.h>

#include "../core.h"

namespace barnack::layout::wrapper
	{

	namespace details
		{
		template <typename slot_T>
		class with_slot_t : public barnack::layout::core::owner
			{
			public:
				using slot_t = slot_T;

				virtual void visit(const std::function<void(const element& element)>& callback) noexcept final override
					{
					element::visit(callback);
					child().visit(callback);
					}

				[[nodiscard]] utils::observer_ptr<slot_t> get_slot(const core::element& previous_element) noexcept
					{
					if (slot.child_own.get() == std::addressof(previous_element))
						{
						return slot;
						}
					return nullptr;
					}

			protected:
				slot_t slot;

				const core::element& child() const noexcept
					{
					assert(this->slot.child_own);
					return *slot.child_own;
					}
				core::element& child() noexcept
					{
					assert(this->slot.child_own);
					return *slot.child_own;
					}

			public:
				virtual void evaluate_traversal_for_children(core::traversal& traversal, core::traversal::node& previously_visited_my_node, const utils::math::vec2f& position) noexcept override
					{
					auto& visited_children{previously_visited_my_node.children};
				
					if (!child().evaluate_traversal(traversal, previously_visited_my_node, position))
						{
						auto it{std::find_if(visited_children.begin(), visited_children.end(), [&](const core::traversal::node& node) { return node.element_obs == std::addressof(child()); })};
						if (it != visited_children.end())
							{
							traversal._left_roots.emplace_back(std::move(*it));
							visited_children.erase(it);
							}
						}
					}
				virtual void evaluate_traversal_for_children(core::traversal::node& my_node, const utils::math::vec2f& position) noexcept override
					{
					child().evaluate_traversal(my_node, position);
					}
			};
		
		template <typename alignment_T>
		class with_alignment : public with_slot_t<core::slot_with_alignment<alignment_T>>
			{
			public:
				using alignment_t = alignment_T;
				with_alignment(alignment_t alignment) : with_slot_t<core::slot_with_alignment<alignment_T>>{alignment}
					{}
			};
		}
	}