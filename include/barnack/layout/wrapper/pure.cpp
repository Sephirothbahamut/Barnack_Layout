#include "pure.h"

namespace barnack::layout::wrapper
	{
	utils::math::vec2f pure::evaluate_size_min() const noexcept { return child().get_size_min(); }
	utils::math::vec2f pure::evaluate_size_prf() const noexcept { return child().get_size_prf(); }
	utils::math::vec2f pure::evaluate_size_max() const noexcept { return child().get_size_max(); }

	void pure::resize(const utils::math::vec2f& max_size)
		{
		layout::element::resize_checked(max_size);
		child().resize(get_rect_content().size());
		}

	void pure::reposition(const utils::math::vec2f& position) noexcept
		{
		layout::element::reposition(position);
		child().reposition(position);
		};
	}