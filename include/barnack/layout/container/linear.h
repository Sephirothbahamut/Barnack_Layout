#pragma once

#include "common.h"

namespace barnack::layout::container
	{
	enum class linear_direction { horizontal, vertical };

	template <linear_direction dir>
	using alignment_for_direction = std::conditional_t<dir == linear_direction::horizontal, utils::alignment::vertical, utils::alignment::horizontal>;

	template <linear_direction DIRECTION>
	class linear : public details::with_alignment<alignment_for_direction<DIRECTION>>
		{
		public:
			inline static constexpr linear_direction direction{DIRECTION};
			using parent_t = details::with_alignment<alignment_for_direction<direction>>;
			using parent_t::with_alignment;
			using parent_t::visit;
			using parent_t::add_slot;
			using parent_t::get_slot;
			
			// For each value, if set in constraints, that one is used. Otherwise, the largest max amongst children is used.
			constraints::bidirectional constraints;

			virtual utils::math::vec2f evaluate_size_min() const noexcept final override;
			virtual utils::math::vec2f evaluate_size_prf() const noexcept final override;
			virtual utils::math::vec2f evaluate_size_max() const noexcept final override;

			virtual void resize    (const utils::math::vec2f& max_size)          final override;
			virtual void reposition(const utils::math::vec2f& position) noexcept final override;

		private:
			std::vector<utils::math::vec2f> evaluate_children_sizes(const utils::math::vec2f& available) noexcept;
		};
	
	using horizontal = linear<linear_direction::horizontal>;
	using vertical   = linear<linear_direction::vertical  >;
	}

#ifdef barnack_implementation
#include "linear.cpp"
#endif