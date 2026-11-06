#pragma once

#include "common.h"

namespace barnack::layout::container::details
	{
	constraints_t constraints_t::hor(const layout::element& element) noexcept
		{
		const utils::math::vec2f min{element.get_size_min()};
		const utils::math::vec2f prf{element.get_size_prf()};
		const utils::math::vec2f max{element.get_size_max()};
		return {min.x(), prf.x(), max.x(), min.y(), prf.y(), max.y()};
		}

	constraints_t constraints_t::ver(const layout::element& element) noexcept
		{
		const utils::math::vec2f min{element.get_size_min()};
		const utils::math::vec2f prf{element.get_size_prf()};
		const utils::math::vec2f max{element.get_size_max()};
		return {min.y(), prf.y(), max.y(), min.x(), prf.x(), max.x()};
		}

	template <typename alignment_t>
	with_alignment<alignment_t>::with_alignment(alignment_t default_alignment) : default_alignment{default_alignment} {}

	template <typename alignment_t>
	bool with_alignment<alignment_t>::visit(const std::function<bool(element& element)>& callback) noexcept
		{
		const bool ret{element::visit(callback)};
		if (ret)
			{
			for_each_child([&](layout::element& child)
				{
				child.visit(callback);
				});
			}
		return ret;
		}

	template <typename alignment_t>
	[[nodiscard]] auto with_alignment<alignment_t>::add_slot(alignment_t slot_alignment) noexcept -> layout::details::slot_access_helper<slot_t>
		{
		return layout::details::slot_access_helper<slot_t>{slots.emplace_back(slot_alignment), *this};
		}

	template <typename alignment_t>
	[[nodiscard]] auto with_alignment<alignment_t>::add_slot() noexcept -> layout::details::slot_access_helper<slot_t>
		{
		return add_slot(default_alignment);
		}

	template <typename alignment_t>
	[[nodiscard]] auto with_alignment<alignment_t>::get_slot(const layout::element& previous_element) noexcept -> std::optional<layout::details::slot_access_helper<slot_t>>
		{
		for (auto& slot : slots)
			{
			if (slot.child_own.get() == std::addressof(previous_element))
				{
				return layout::details::slot_access_helper<slot_t>{slot, *this};
				}
			}
		return std::nullopt;
		}

	template <typename alignment_t>
	void with_alignment<alignment_t>::evaluate_traversal_for_children(layout::traversal& traversal, layout::traversal::node& previously_visited_my_node, const utils::math::vec2f& position) noexcept
		{
		auto& visited_children{previously_visited_my_node.children};
		for_each_child([&](element& child)
			{
			if (!child.evaluate_traversal(traversal, previously_visited_my_node, position))
				{
				auto it{std::find_if(visited_children.begin(), visited_children.end(), [&](const layout::traversal::node& node) { return node.element_obs == std::addressof(child); })};
				if (it != visited_children.end())
					{
					traversal._left_roots.emplace_back(std::move(*it));
					visited_children.erase(it);
					}
				}
			});
		}

	template <typename alignment_t>
	void with_alignment<alignment_t>::evaluate_traversal_for_children(layout::traversal::node& my_node, const utils::math::vec2f& position) noexcept
		{
		for_each_child([&](element& child)
			{
			child.evaluate_traversal(my_node, position);
			});
		}

	std::vector<std::pair<float, float>> calc_sizes(std::pair<float, float> available, std::vector<constraints_t>& constraints) noexcept
		{
		//First/a represents the size along the axis on which the container expands (vertical for vertical container)
		//Second/b represents the size along the axis on which the container always has 1 element (horizontal for vertical container)
		std::vector<std::pair<float, float>> results; results.resize(constraints.size());

		// Remove the minimums from available space
		// Update max and desired removing their minimum offset
		for (auto& constraint : constraints)
			{
			constraint.max_a -= constraint.min_a;
			constraint.prf_a -= constraint.min_a;
			}

		// Calc total sums and available without minimum offset
		float mins_sum{0};
		float maxs_sum{0};
		float prfs_sum{0};

		for (auto& constraint : constraints)
			{
			//Get rid of infinities (proportions don't like 'em :) )
			if (constraint.max_a == utils::math::constants::finf)
				{
				constraint.max_a = available.first;
				constraint.prf_a = std::min(constraint.prf_a, constraint.max_a);
				}
			else if (constraint.prf_a == utils::math::constants::finf)
				{
				constraint.prf_a = std::min(constraint.prf_a, constraint.max_a);
				}
			mins_sum += constraint.min_a;
			maxs_sum += constraint.max_a;
			prfs_sum += constraint.prf_a;
			}

		available.first -= mins_sum;
		available.first = std::min(available.first, maxs_sum);

		//Setup base results
		float still_available{available.first - prfs_sum};

		for (const auto& [index, constraint] : std::views::enumerate(constraints))
			{
			auto& result{results[index]};
			result.first = constraint.min_a + constraint.prf_a;
			result.second = std::min(available.second, constraint.max_b);
			}

		//Let ze magic happen
		if (still_available == 0) [[unlikely]] {}
		else if (still_available > 0) // more space is available than required to all elements to reach their preferred
			{
			// |                 v    | 'v': available delimiter
			// |[****P**][*P********] | '[': min, 'p': preferred, ']' max
			// |[****P***][*P***]     | if we distribute the delta equally amongst all items, we may go past maximums
			// |[****P*.][*P****.]    | '.': is a fraciton of a unit.

			float delta_sum{maxs_sum - prfs_sum};
			for (const auto& [index, constraint] : std::views::enumerate(constraints))
				{
				auto& result{results[index]};

				float delta{constraint.max_a - constraint.prf_a};
				float proportion{delta / delta_sum};
				float out{proportion * still_available};

				result.first += out;
				}
			}
		else if (still_available < 0) // no enough space is available for all elements to reach their preferred
			{
			float delta_sum{prfs_sum};

			for (const auto& [index, constraint] : std::views::enumerate(constraints))
				{
				auto& result{results[index]};

				float delta{constraint.prf_a};
				float proportion{delta / delta_sum};
				float out{proportion * still_available};

				result.first += out;
				}
			}

		return results;
		}


	template class with_alignment<utils::alignment            >;
	template class with_alignment<utils::alignment::vertical  >;
	template class with_alignment<utils::alignment::horizontal>;
	}