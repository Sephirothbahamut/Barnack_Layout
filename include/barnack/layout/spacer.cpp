#include "spacer.h"

namespace barnack::layout
	{
	spacer::spacer(constraints::monodirectional constraints) noexcept : constraints{constraints, constraints} {}
	spacer::spacer(constraints::bidirectional   constraints) noexcept : constraints{constraints} {}

	utils::math::vec2f spacer::evaluate_size_min() const noexcept 
		{
		return
			{
			std::isnan(constraints.x.get_min()) ? 0.f : constraints.x.get_min(),
			std::isnan(constraints.y.get_min()) ? 0.f : constraints.y.get_min()
			};
		}
	utils::math::vec2f spacer::evaluate_size_prf() const noexcept
		{
		const auto avg{(get_size_min() + get_size_max()) / 2.f};
		return
			{
			std::isnan(constraints.x.get_prf()) ? avg.x() : constraints.x.get_prf(),
			std::isnan(constraints.y.get_prf()) ? avg.y() : constraints.y.get_prf()
			};
		}
	utils::math::vec2f spacer::evaluate_size_max() const noexcept
		{
		return
			{
			std::isnan(constraints.x.get_max()) ? utils::math::constants::finf : constraints.x.get_max(),
			std::isnan(constraints.y.get_max()) ? utils::math::constants::finf : constraints.y.get_max()
			};
		}
	}

