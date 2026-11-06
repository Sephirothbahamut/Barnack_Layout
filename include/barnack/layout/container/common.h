#pragma once

#include <ranges>

#include <utils/alignment.h>
#include <utils/oop/disable_move_copy.h>

#include "../element.h"
#include "../slot.h"

namespace barnack::layout::container
	{
	namespace details
		{
		struct constraints_t
			{
			float min_a{0.f};
			float prf_a{0.f};
			float max_a{0.f};
			float min_b{0.f};
			float prf_b{0.f};
			float max_b{0.f};

			static constraints_t hor(const layout::element& element) noexcept;

			static constraints_t ver(const layout::element& element) noexcept;
			};

		template <typename alignment_T>
		class with_alignment : public barnack::layout::owner
			{
			public:
				using alignment_t = alignment_T;
				using slot_t = layout::slot_with_alignment<alignment_t>;
				alignment_t default_alignment{utils::alignment::default_value<alignment_t>()};

				with_alignment() = default;
				with_alignment(alignment_t default_alignment);

				virtual bool visit(const std::function<bool(element& element)>& callback) noexcept override;

				[[nodiscard]] layout::details::slot_access_helper<slot_t> add_slot(alignment_t slot_alignment) noexcept;
				[[nodiscard]] layout::details::slot_access_helper<slot_t> add_slot() noexcept;

				[[nodiscard]] std::optional<layout::details::slot_access_helper<slot_t>> get_slot(const layout::element& previous_element) noexcept;

			protected:
				std::vector<layout::slot_with_alignment<alignment_t>> slots;
				
				void for_each_slot(auto/*void(const slot_t&)*/ callback) const noexcept
					{
					for (const auto& slot : slots)
						{
						callback(slot);
						}
					}

				void for_each_slot(auto/*void(const slot_t&)*/ callback) noexcept
					{
					for (auto& slot : slots)
						{
						callback(slot);
						}
					}

				void for_each_child(auto/*void(const layout::element&)*/ callback) const noexcept
					{
					for_each_slot([&](const slot_t& slot)
						{
						assert(slot.child_own);
						callback(*slot.child_own);
						});
					}

				void for_each_child(auto/*void(const layout::element&)*/ callback) noexcept
					{
					for_each_slot([&](slot_t& slot)
						{
						assert(slot.child_own);
						callback(*slot.child_own);
						});
					}

			public:
				virtual void evaluate_traversal_for_children(layout::traversal& traversal, layout::traversal::node& previously_visited_my_node, const utils::math::vec2f& position) noexcept override;
				virtual void evaluate_traversal_for_children(layout::traversal::node& my_node, const utils::math::vec2f& position) noexcept override;
			};

		std::vector<std::pair<float, float>> calc_sizes(std::pair<float, float> available, std::vector<constraints_t>& constraints) noexcept;
		}
	}

#ifdef barnack_implementation
#include "common.cpp"
#endif