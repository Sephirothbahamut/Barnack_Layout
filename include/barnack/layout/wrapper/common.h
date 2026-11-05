#pragma once

#include <utils/alignment.h>

#include "../element.h"
#include "../slot.h"

namespace barnack::layout::wrapper::details
	{
	template <typename slot_T>
	class with_slot_t : public barnack::layout::owner
		{
		public:
			using slot_t = slot_T;

			with_slot_t() = default;

			template <typename alignment_t>
			with_slot_t(alignment_t alignment) requires std::same_as<slot_t, layout::slot_with_alignment<alignment_t>> : slot{alignment} {}

			virtual void visit(const std::function<void(const element& element)>& callback) noexcept final override;

			[[nodiscard]] layout::details::slot_access_helper<slot_t> get_slot() noexcept;
			[[nodiscard]] std::optional<layout::details::slot_access_helper<slot_t>> get_slot(const layout::element& previous_element) noexcept;

		protected:
			slot_t slot;

			const layout::element& child() const noexcept;
			layout::element& child() noexcept;

		public:
			virtual void evaluate_traversal_for_children(layout::traversal& traversal, layout::traversal::node& previously_visited_my_node, const utils::math::vec2f& position) noexcept final override;
			virtual void evaluate_traversal_for_children(layout::traversal::node& my_node, const utils::math::vec2f& position) noexcept final override;
		};
		
	template <typename alignment_T>
	class with_alignment : public with_slot_t<layout::slot_with_alignment<alignment_T>>
		{
		public:
			using alignment_t = alignment_T;
			with_alignment(alignment_t alignment);
		};
	}

#ifdef barnack_layout_implementation
#include "common.cpp"
#endif