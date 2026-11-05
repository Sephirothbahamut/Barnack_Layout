#include "spacing.h"

namespace barnack::layout::wrapper
	{
	utils::math::vec2f spacing_fixed::evaluate_size_min() const noexcept { return child().get_size_min() + spacing; }
	utils::math::vec2f spacing_fixed::evaluate_size_prf() const noexcept { return child().get_size_prf() + spacing; }
	utils::math::vec2f spacing_fixed::evaluate_size_max() const noexcept { return child().get_size_max() + spacing; }

	void spacing_fixed::resize(const utils::math::vec2f& max_size)
		{
		layout::element::resize_checked(max_size);
		child().resize(get_rect_content().size() - spacing);
		}

	void spacing_fixed::reposition(utils::math::vec2f position) noexcept
		{
		layout::element::reposition(position);
		child().reposition(get_rect_content().position() + utils::math::vec2f{spacing.ll(), spacing.up()});
		}
	}