#pragma once

#include <ranges>

#include <utils/alignment.h>

#include "../core.h"

namespace barnack::layout::containers
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
			utils::math::vec2f min{element.get_size_min()};
			utils::math::vec2f prf{element.get_size_prf()};
			utils::math::vec2f max{element.get_size_max()};
			return {min.x(), prf.x(), max.x(), min.y(), prf.y(), max.y()};
			}

		static constraints_t ver(const core::element& element)
			{
			utils::math::vec2f min{element.get_size_min()};
			utils::math::vec2f prf{element.get_size_prf()};
			utils::math::vec2f max{element.get_size_max()};
			return {min.y(), prf.y(), max.y(), min.x(), prf.x(), max.x()};
			}
		};

	std::vector<std::pair<float, float>> calc_sizes(std::pair<float, float> available, std::vector<constraints_t>& constraints) noexcept
		{
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