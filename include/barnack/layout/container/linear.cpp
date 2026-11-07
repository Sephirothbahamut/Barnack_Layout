#include "linear.h"

namespace barnack::layout::container
	{
	//H symbolizes two parrallel lines, get_parallel_to_direction
	//T symbolizes two perpendicular lines, get_perpendicular_to_direction
	//I just want them short here and they're only used here so it's okey

	template <linear_direction dir>
	float& H(utils::math::vec2f& source) noexcept
		{
		if constexpr (dir == linear_direction::horizontal) { return source.x(); }
		if constexpr (dir == linear_direction::vertical  ) { return source.y(); }
		}
	template <linear_direction dir>
	float H(const utils::math::vec2f& source) noexcept
		{
		if constexpr (dir == linear_direction::horizontal) { return source.x(); }
		if constexpr (dir == linear_direction::vertical  ) { return source.y(); }
		}
	template <linear_direction dir>
	float& T(utils::math::vec2f& source) noexcept
		{
		if constexpr (dir == linear_direction::horizontal) { return source.y(); }
		if constexpr (dir == linear_direction::vertical  ) { return source.x(); }
		}
	template <linear_direction dir>
	float T(const utils::math::vec2f& source) noexcept
		{
		if constexpr (dir == linear_direction::horizontal) { return source.y(); }
		if constexpr (dir == linear_direction::vertical  ) { return source.x(); }
		}


	struct unchecked_constaints_t
		{
		utils::math::vec2f min;
		utils::math::vec2f max;
		utils::math::vec2f prf;
		};














	template <linear_direction direction>
	std::vector<utils::math::vec2f> linear<direction>::evaluate_children_sizes(const utils::math::vec2f& available) noexcept
		{
		std::vector<unchecked_constaints_t> constraints; constraints.resize(this->slots.size());
		std::vector<utils::math::vec2f    > results    ; results    .resize(this->slots.size());

		// Remove the minimums from available space
		// Update max and desired removing their minimum offset
		for (size_t i{0}; i < this->slots.size(); i++)
			{
			const auto& slot{this->slots[i]};
			const auto& child_own{slot.child_own};
			const auto& child{*child_own};

			auto& constraint{constraints[i]};

			const auto tmp_min{child.get_size_min()};
			const auto tmp_prf{child.get_size_prf()};
			const auto tmp_max{child.get_size_max()};

			H<direction>(constraint.min) = H<direction>(tmp_min);
			H<direction>(constraint.prf) = H<direction>(tmp_prf) - H<direction>(constraint.min);
			H<direction>(constraint.max) = H<direction>(tmp_max) - H<direction>(constraint.min);

			T<direction>(constraint.min) = T<direction>(tmp_min);
			T<direction>(constraint.prf) = T<direction>(tmp_prf);
			T<direction>(constraint.max) = T<direction>(tmp_max);
			}

		// Calc total sums and available without minimum offset
		float mins_sum{0};
		float maxs_sum{0};
		float prfs_sum{0};

		for (auto& constraint : constraints)
			{
			//Get rid of infinities (proportions don't like 'em :) )
			if (H<direction>(constraint.max) == utils::math::constants::finf)
				{
				H<direction>(constraint.max) = H<direction>(available);
				H<direction>(constraint.prf) = std::min<float>(H<direction>(constraint.prf), H<direction>(constraint.max));
				}
			else if (H<direction>(constraint.prf) == utils::math::constants::finf)
				{
				H<direction>(constraint.prf) = std::min<float>(H<direction>(constraint.prf), H<direction>(constraint.max));
				}
			mins_sum += H<direction>(constraint.min);
			maxs_sum += H<direction>(constraint.max);
			prfs_sum += H<direction>(constraint.prf);
			}

		float parallel_available{H<direction>(available)};
		parallel_available -= mins_sum;
		parallel_available = std::min(parallel_available, maxs_sum);

		//Setup base results
		float still_available{parallel_available - prfs_sum};

		for (const auto& [index, constraint] : std::views::enumerate(constraints))
			{
			auto& result{results[index]};
			H<direction>(result) = H<direction>(constraint.min) + H<direction>(constraint.prf);
			T<direction>(result) = std::min<float>(static_cast<float>(T<direction>(available)), T<direction>(constraint.max));
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

				float delta{H<direction>(constraint.max) - H<direction>(constraint.prf)};
				float proportion{delta / delta_sum};
				float out{proportion * still_available};

				H<direction>(result) += out;
				}
			}
		else if (still_available < 0) // no enough space is available for all elements to reach their preferred
			{
			float delta_sum{prfs_sum};

			for (const auto& [index, constraint] : std::views::enumerate(constraints))
				{
				auto& result{results[index]};

				float delta{H<direction>(constraint.prf)};
				float proportion{delta / delta_sum};
				float out{proportion * still_available};

				H<direction>(result) += out;
				}
			}

		return results;
		}
















	template <linear_direction direction>
	utils::math::vec2f linear<direction>::evaluate_size_min() const noexcept
		{
		utils::math::vec2f ret{0.f, 0.f};
		this->for_each_child([&](const layout::element& child)
			{
			utils::math::vec2f child_val{child.get_size_min()};
			H<direction>(ret) += H<direction>(child_val);
			T<direction>(ret) = std::max(T<direction>(ret), T<direction>(child_val));
			});

		return ret.operator_to_new(constraints.get_min(), [](const auto& a, const auto& b) 
			{
			if (std::isnan(b)) { return a; }
			return utils::math::max(a, b); 
			});
		}

	template <linear_direction direction>
	utils::math::vec2f linear<direction>::evaluate_size_prf() const noexcept
		{
		utils::math::vec2f ret{utils::math::constants::fnan, utils::math::constants::fnan};
		this->for_each_child([&](const layout::element& child)
			{
			utils::math::vec2f child_val{child.get_size_prf()};
			H<direction>(ret) = layout::details::add_nan_aware(H<direction>(ret), H<direction>(child_val));
			T<direction>(ret) = layout::details::max_nan_aware(T<direction>(ret), T<direction>(child_val));
			});
		return ret.operator_to_new(constraints.get_prf(), [](const auto& a, const auto& b)
			{
			if (std::isnan(b)) { return a; }
			return utils::math::max(a, b);
			});
		}

	template <linear_direction direction>
	utils::math::vec2f linear<direction>::evaluate_size_max() const noexcept
		{
		if (this->slots.empty()) { return {utils::math::constants::finf, utils::math::constants::finf}; }

		utils::math::vec2f ret{0.f, 0.f};
		this->for_each_child([&](const layout::element& child)
			{
			utils::math::vec2f child_val{child.get_size_max()};
			H<direction>(ret) += H<direction>(child_val);
			T<direction>(ret) = std::max(T<direction>(ret), T<direction>(child_val));
			});
		return ret.operator_to_new(constraints.get_max(), [](const auto& a, const auto& b)
			{
			if (std::isnan(b)) { return a; }
			return utils::math::max(a, b);
			});
		}

	template <linear_direction direction>
	void linear<direction>::resize(const utils::math::vec2f& max_size)
		{
		layout::element::resize_checked(max_size);

		auto sizes{evaluate_children_sizes(this->get_rect_content().size())};

		for (size_t i{0}; i < this->slots.size(); i++)
			{
			auto& child_own{this->slots[i].child_own};
			assert(child_own);
			auto& child{*child_own};
			const auto& size{sizes[i]};
			child.resize(size);
			}
		}

	template <linear_direction direction>
	void linear<direction>::reposition(const utils::math::vec2f& position) noexcept
		{
		layout::element::reposition(position);

		float delta_parallel{0};
		this->for_each_slot([&](const linear<direction>::slot_t& slot)
			{
			auto& child_own{slot.child_own};
			assert(child_own);
			auto& child{*child_own};

			const float delta_perpendicular{layout::details::evaluate_element_position_for_alignment_based_container(this->get_rect_content(), child.get_rect_content(), slot.alignment)};

			utils::math::vec2f element_new_position;
			H<direction>(element_new_position) = H<direction>(position) + delta_parallel;
			T<direction>(element_new_position) = T<direction>(position) + delta_perpendicular;

			child.reposition(element_new_position);
			delta_parallel += H<direction>(child.get_rect_content().size());
			});
		}


	template class linear<linear_direction::horizontal>;
	template class linear<linear_direction::vertical  >;
	}