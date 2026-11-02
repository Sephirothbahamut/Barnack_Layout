#pragma once

#include "common.h"

#include "../core.h"

namespace barnack::layout::wrapper
	{
	struct filler_horizontal : public details::with_alignment<utils::alignment::horizontal>
		{
		virtual utils::math::vec2f evaluate_size_min() const noexcept final override 
			{
			return child().get_size_min();
			}
		virtual utils::math::vec2f evaluate_size_prf() const noexcept final override
			{
			return child().get_size_prf();
			}
		virtual utils::math::vec2f evaluate_size_max() const noexcept final override
			{
			return {utils::math::constants::finf, child().get_size_max().y()};
			}

		virtual void resize(const utils::math::vec2f& max_size)  final override
			{
			core::element::resize_checked(max_size);

			const float width{std::min(child().get_size_max().x(), max_size.x())};

			child().resize({width, rect_content.height()});
			}
		virtual void reposition(const utils::math::vec2f& position) noexcept override
			{
			core::element::reposition(position);

			const float delta_x{layout::details::evaluate_element_position_for_alignment_based_container(rect_content, child().rect_content, slot.alignment)};

			const utils::math::vec2f element_new_position{rect_content.x() + delta_x, rect_content.y()};

			child().reposition(element_new_position);
			};
		};

	struct filler_vertical : public details::with_alignment<utils::alignment::vertical>
		{
		virtual utils::math::vec2f evaluate_size_min() const noexcept final override
			{
			return child().get_size_min();
			}
		virtual utils::math::vec2f evaluate_size_prf() const noexcept final override
			{
			return child().get_size_prf();
			}
		virtual utils::math::vec2f evaluate_size_max() const noexcept final override
			{
			return {child().get_size_max().x(), utils::math::constants::finf};
			}

		virtual void resize(const utils::math::vec2f& max_size)  final override
			{
			core::element::resize_checked(max_size);

			const float height{std::min(child().get_size_max().x(), max_size.x())};

			child().resize({rect_content.width(), height});
			}
		virtual void reposition(const utils::math::vec2f& position) noexcept override
			{
			core::element::reposition(position);

			const float delta_y{layout::details::evaluate_element_position_for_alignment_based_container(rect_content, child().rect_content, slot.alignment)};

			const utils::math::vec2f element_new_position{rect_content.x(), rect_content.y() + delta_y};

			child().reposition(element_new_position);
			};
		};

	struct filler_bidirectional : public details::with_alignment<utils::alignment>
		{
		virtual utils::math::vec2f evaluate_size_min() const noexcept final override
			{
			return child().get_size_min();
			}
		virtual utils::math::vec2f evaluate_size_prf() const noexcept final override
			{
			return child().get_size_prf();
			}
		virtual utils::math::vec2f evaluate_size_max() const noexcept final override
			{
			return {utils::math::constants::finf, utils::math::constants::finf};
			}

		virtual void resize(const utils::math::vec2f& max_size)  final override
			{
			core::element::resize_checked(max_size);

			const float width {std::min(child().get_size_max().x(), max_size.x())};
			const float height{std::min(child().get_size_max().x(), max_size.x())};

			child().resize({width, height});
			}

		virtual void reposition(const utils::math::vec2f& position) noexcept override
			{
			core::element::reposition(position);

			const float delta_x{layout::details::evaluate_element_position_for_alignment_based_container(rect_content, child().rect_content, slot.alignment.horizontal_alignment)};
			const float delta_y{layout::details::evaluate_element_position_for_alignment_based_container(rect_content, child().rect_content, slot.alignment.vertical_alignment  )};

			const utils::math::vec2f element_new_position{rect_content.x() + delta_x, rect_content.y() + delta_y};

			child().reposition(element_new_position);
			};
		};
	}