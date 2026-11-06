#include "filler.h"

namespace barnack::layout::wrapper
	{
	utils::math::vec2f filler_horizontal::evaluate_size_min() const noexcept
		{
		return child().get_size_min();
		}
	utils::math::vec2f filler_horizontal::evaluate_size_prf() const noexcept
		{
		return child().get_size_prf();
		}
	utils::math::vec2f filler_horizontal::evaluate_size_max() const noexcept
		{
		return {utils::math::constants::finf, child().get_size_max().y()};
		}

	void filler_horizontal::resize(const utils::math::vec2f& max_size) 
		{
		layout::element::resize_checked(max_size);

		const float width{std::min(child().get_size_max().x(), max_size.x())};

		child().resize({width, get_rect_content().height()});
		}
	void filler_horizontal::reposition(const utils::math::vec2f& position) noexcept
		{
		layout::element::reposition(position);

		const float delta_x{layout::details::evaluate_element_position_for_alignment_based_container(get_rect_content(), child().get_rect_content(), slot.alignment)};

		const utils::math::vec2f element_new_position{get_rect_content().x() + delta_x, get_rect_content().y()};

		child().reposition(element_new_position);
		}


	utils::math::vec2f filler_vertical::evaluate_size_min() const noexcept
		{
		return child().get_size_min();
		}
	utils::math::vec2f filler_vertical::evaluate_size_prf() const noexcept
		{
		return child().get_size_prf();
		}
	utils::math::vec2f filler_vertical::evaluate_size_max() const noexcept
		{
		return {child().get_size_max().x(), utils::math::constants::finf};
		}

	void filler_vertical::resize(const utils::math::vec2f& max_size) 
		{
		layout::element::resize_checked(max_size);

		const float height{std::min(child().get_size_max().x(), max_size.x())};

		child().resize({get_rect_content().width(), height});
		}
	void filler_vertical::reposition(const utils::math::vec2f& position) noexcept
		{
		layout::element::reposition(position);

		const float delta_y{layout::details::evaluate_element_position_for_alignment_based_container(get_rect_content(), child().get_rect_content(), slot.alignment)};

		const utils::math::vec2f element_new_position{get_rect_content().x(), get_rect_content().y() + delta_y};
		child().reposition(element_new_position);
		}

	utils::math::vec2f filler_bidirectional::evaluate_size_min() const noexcept
		{
		return child().get_size_min();
		}
	utils::math::vec2f filler_bidirectional::evaluate_size_prf() const noexcept
		{
		return child().get_size_prf();
		}
	utils::math::vec2f filler_bidirectional::evaluate_size_max() const noexcept
		{
		return {utils::math::constants::finf, utils::math::constants::finf};
		}

	void filler_bidirectional::resize(const utils::math::vec2f& max_size) 
		{
		layout::element::resize_checked(max_size);

		const float width {std::min(child().get_size_max().x(), max_size.x())};
		const float height{std::min(child().get_size_max().x(), max_size.x())};

		child().resize({width, height});
		}

	void filler_bidirectional::reposition(const utils::math::vec2f& position) noexcept
		{
		layout::element::reposition(position);

		const float delta_x{layout::details::evaluate_element_position_for_alignment_based_container(get_rect_content(), child().get_rect_content(), slot.alignment.horizontal_alignment)};
		const float delta_y{layout::details::evaluate_element_position_for_alignment_based_container(get_rect_content(), child().get_rect_content(), slot.alignment.vertical_alignment  )};

		const utils::math::vec2f element_new_position{get_rect_content().x() + delta_x, get_rect_content().y() + delta_y};

		child().reposition(element_new_position);
		}
	}