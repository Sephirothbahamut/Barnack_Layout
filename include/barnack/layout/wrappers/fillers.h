#pragma once

#include <utils/alignment.h>

#include "../core.h"

namespace barnack::layout::wrappers
	{
	struct filler_horizontal : public core::wrapper
		{
		utils::alignment::horizontal alignment;

		virtual utils::math::vec2f evaluate_size_min() const noexcept final override 
			{
			return element_ptr->get_size_min();
			}
		virtual utils::math::vec2f evaluate_size_prf() const noexcept final override
			{
			return element_ptr->get_size_prf();
			}
		virtual utils::math::vec2f evaluate_size_max() const noexcept final override
			{
			return {utils::math::constants::finf, element_ptr->get_size_max().y()};
			}

		virtual void resize(const utils::math::vec2f& max_size)  final override
			{
			core::element::resize_checked(max_size);

			const float width{std::min(element_ptr->get_size_max().x(), max_size.x())};

			element_ptr->resize({width, rect.height()});
			}
		virtual void reposition(const utils::math::vec2f& position) noexcept override
			{
			core::element::reposition(position);

			const float delta_x{layout::details::evaluate_element_position_for_alignment_based_container(rect, element_ptr->rect, alignment)};

			const utils::math::vec2f element_new_position{rect.x() + delta_x, rect.y()};

			element_ptr->reposition(element_new_position);
			};
		};

	struct filler_vertical: public core::wrapper
		{
		utils::alignment::vertical alignment;

		virtual utils::math::vec2f evaluate_size_min() const noexcept final override
			{
			return element_ptr->get_size_min();
			}
		virtual utils::math::vec2f evaluate_size_prf() const noexcept final override
			{
			return element_ptr->get_size_prf();
			}
		virtual utils::math::vec2f evaluate_size_max() const noexcept final override
			{
			return {element_ptr->get_size_max().x(), utils::math::constants::finf};
			}

		virtual void resize(const utils::math::vec2f& max_size)  final override
			{
			core::element::resize_checked(max_size);

			const float height{std::min(element_ptr->get_size_max().x(), max_size.x())};

			element_ptr->resize({rect.width(), height});
			}
		virtual void reposition(const utils::math::vec2f& position) noexcept override
			{
			core::element::reposition(position);

			const float delta_y{layout::details::evaluate_element_position_for_alignment_based_container(rect, element_ptr->rect, alignment)};

			const utils::math::vec2f element_new_position{rect.x(), rect.y() + delta_y};

			element_ptr->reposition(element_new_position);
			};
		};

	struct filler_bidirectional : public core::wrapper
		{
		utils::alignment alignment;

		virtual utils::math::vec2f evaluate_size_min() const noexcept final override
			{
			return element_ptr->get_size_min();
			}
		virtual utils::math::vec2f evaluate_size_prf() const noexcept final override
			{
			return element_ptr->get_size_prf();
			}
		virtual utils::math::vec2f evaluate_size_max() const noexcept final override
			{
			return {utils::math::constants::finf, utils::math::constants::finf};
			}

		virtual void resize(const utils::math::vec2f& max_size)  final override
			{
			core::element::resize_checked(max_size);

			const float width {std::min(element_ptr->get_size_max().x(), max_size.x())};
			const float height{std::min(element_ptr->get_size_max().x(), max_size.x())};

			element_ptr->resize({width, height});
			}

		virtual void reposition(const utils::math::vec2f& position) noexcept override
			{
			core::element::reposition(position);

			const float delta_x{layout::details::evaluate_element_position_for_alignment_based_container(rect, element_ptr->rect, alignment.horizontal_alignment)};
			const float delta_y{layout::details::evaluate_element_position_for_alignment_based_container(rect, element_ptr->rect, alignment.vertical_alignment  )};

			const utils::math::vec2f element_new_position{rect.x() + delta_x, rect.y() + delta_y};

			element_ptr->reposition(element_new_position);
			};
		};
	}