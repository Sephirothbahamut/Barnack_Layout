#pragma once

#include <ranges>

#include <utils/alignment.h>
#include <utils/oop/disable_move_copy.h>

#include "../core.h"

namespace barnack::layout::container
	{
	struct constraints_t
		{
		float min_a{0.f};
		float prf_a{0.f};
		float max_a{0.f};
		float min_b{0.f};
		float prf_b{0.f};
		float max_b{0.f};

		static constraints_t hor(const core::element& element)
			{
			const utils::math::vec2f min{element.get_size_min()};
			const utils::math::vec2f prf{element.get_size_prf()};
			const utils::math::vec2f max{element.get_size_max()};
			return {min.x(), prf.x(), max.x(), min.y(), prf.y(), max.y()};
			}

		static constraints_t ver(const core::element& element)
			{
			const utils::math::vec2f min{element.get_size_min()};
			const utils::math::vec2f prf{element.get_size_prf()};
			const utils::math::vec2f max{element.get_size_max()};
			return {min.y(), prf.y(), max.y(), min.x(), prf.x(), max.x()};
			}
		};
	
	namespace details
		{
		template <typename alignment_T>
		class with_alignment : public barnack::layout::core::owner
			{
			public:
				using alignment_t = alignment_T;
				using slot_t = core::slot_with_alignment<alignment_t>;
				alignment_t default_alignment{utils::alignment::default_value<alignment_t>()};

				with_alignment() = default;
				with_alignment(alignment_t default_alignment) : default_alignment{default_alignment} {}

				virtual void visit(const std::function<void(const element& element)>& callback) noexcept final override
					{
					element::visit(callback);
					for_each_child([&](core::element& child) { child.visit(callback); });
					}

				[[nodiscard]] auto add_slot(alignment_t slot_alignment) noexcept
					{
					return core::details::emplacement_helper<slot_t>{slots.emplace_back(slot_alignment)};
					}

				[[nodiscard]] auto add_slot() noexcept
					{
					return add_slot(default_alignment);
					}

				[[nodiscard]] utils::observer_ptr<slot_t> get_slot(const core::element& previous_element) noexcept
					{
					for (auto& slot : slots)
						{
						if (slot.child_own.get() == std::addressof(previous_element))
							{
							return std::addressof(slot);
							}
						}
					return nullptr;
					}

			protected:
				std::vector<core::slot_with_alignment<alignment_t>> slots;

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

				void for_each_child(auto/*void(const core::element&)*/ callback) const noexcept
					{
					for_each_slot([&](const slot_t& slot)
						{
						assert(slot.child_own);
						callback(*slot.child_own);
						});
					}
				void for_each_child(auto/*void(const core::element&)*/ callback) noexcept
					{
					for_each_slot([&](slot_t& slot)
						{
						assert(slot.child_own);
						callback(*slot.child_own);
						});
					}

			public:
				virtual void evaluate_traversal_for_children(core::traversal& traversal, core::traversal::node& previously_visited_my_node, const utils::math::vec2f& position) noexcept override
					{
					auto& visited_children{previously_visited_my_node.children};
					for_each_child([&](element& child)
						{
						if (!child.evaluate_traversal(traversal, previously_visited_my_node, position))
							{
							auto it{std::find_if(visited_children.begin(), visited_children.end(), [&](const core::traversal::node& node) { return node.element_obs == std::addressof(child); })};
							if (it != visited_children.end())
								{
								traversal._left_roots.emplace_back(std::move(*it));
								visited_children.erase(it);
								}
							}
						});
					}
				virtual void evaluate_traversal_for_children(core::traversal::node& my_node, const utils::math::vec2f& position) noexcept override
					{
					for_each_child([&](element& child)
						{
						child.evaluate_traversal(my_node, position);
						});
					}
			};

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
		}
	}