#include "filler.h"

namespace barnack::layout::wrapper
	{
	//Note for future me: the evaluation pairs seems trange but make sense
	// For min we want the maximum between the two minimums. Seems weird but it's okey. This guarantees that the custom minimum can fit the content.
	// For prf we want the custom preferred clamped by the evaluated minimum and maximum.
	// For max we want the maximum between the custom maximum and the content's *minimum*. Seems weird but it's okey. This guarantees that the custom maximum can fit the content.

	utils::math::vec2f filler_horizontal::evaluate_size_min() const noexcept
		{
		return 
			{
			layout::details::max_nan_aware(child().get_size_min().x(), constraints.get_min()), 
			child().get_size_min().y()
			};
		}
	utils::math::vec2f filler_horizontal::evaluate_size_prf() const noexcept
		{
		const auto child_prf{child().get_size_prf()};
		return
			{
			std::isnan(constraints.get_prf()) ? child_prf.x() : std::clamp(constraints.get_prf(), get_size_min().x(), get_size_max().x()),
			child_prf.y()
			};
		}
	utils::math::vec2f filler_horizontal::evaluate_size_max() const noexcept
		{
		return 
			{
			layout::details::max_nan_aware(child().get_size_min().x(), constraints.get_max()), 
			child().get_size_max().y()
			};
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
		return
			{
			child().get_size_min().x(),
			layout::details::max_nan_aware(child().get_size_min().y(), constraints.get_min())
			};
		}
	utils::math::vec2f filler_vertical::evaluate_size_prf() const noexcept
		{
		const auto child_prf{child().get_size_prf()};
		return
			{
			child_prf.y(),
			std::isnan(constraints.get_prf()) ? child_prf.y() : std::clamp(constraints.get_prf(), get_size_min().y(), get_size_max().y())
			};
		}
	utils::math::vec2f filler_vertical::evaluate_size_max() const noexcept
		{
		return
			{
			child().get_size_max().x(),
			layout::details::max_nan_aware(child().get_size_min().y(), constraints.get_max())
			};
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
		return layout::details::max_nan_aware(child().get_size_min(), constraints.get_min());
		}
	utils::math::vec2f filler_bidirectional::evaluate_size_prf() const noexcept
		{
		return layout::details::max_nan_aware(child().get_size_prf(), constraints.get_prf());
		}
	utils::math::vec2f filler_bidirectional::evaluate_size_max() const noexcept
		{
		return layout::details::max_nan_aware(child().get_size_min(), constraints.get_max());
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