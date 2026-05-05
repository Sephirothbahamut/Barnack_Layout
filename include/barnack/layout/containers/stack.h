#pragma once

#include "common.h"

namespace barnack::layout::containers
	{
	struct stack : public core::container
		{
		utils::alignment alignment{utils::alignment::create::top_left()};

		virtual utils::math::vec2f evaluate_size_min() const noexcept final override
			{
			utils::math::vec2f ret{0.f, 0.f};
			for (const auto& element_ptr : elements)
				{
				utils::math::vec2f element_val{element_ptr->get_size_min()};
				ret.x() = std::max(ret.x(), element_val.x());
				ret.y() = std::max(ret.y(), element_val.y());
				}
			return ret;
			}
		virtual utils::math::vec2f evaluate_size_prf() const noexcept final override
			{
			utils::math::vec2f ret{0.f, 0.f};
			for (const auto& element_ptr : elements)
				{
				utils::math::vec2f element_val{element_ptr->get_size_prf()};
				ret.x() = std::max(ret.x(), element_val.x());
				ret.y() = std::max(ret.y(), element_val.y());
				}
			return ret;
			}
		virtual utils::math::vec2f evaluate_size_max() const noexcept final override
			{
			utils::math::vec2f ret{0.f, 0.f};
			for (const auto& element_ptr : elements)
				{
				utils::math::vec2f element_val{element_ptr->get_size_max()};
				ret.x() = std::max(ret.x(), element_val.x());
				ret.y() = std::max(ret.y(), element_val.y());
				}
			return ret;
			}

		virtual void resize(const utils::math::vec2f& max_size)  final override
			{
			core::element::resize_checked(max_size);

			for (size_t i{0}; i < elements.size(); i++)
				{
				auto& element{*elements[i]};
				element.resize_checked(rect.size());
				}
			}
		virtual void reposition(const utils::math::vec2f& position) noexcept override
			{
			core::element::reposition(position);

			for (auto& element_ptr : elements)
				{
				auto& element{*element_ptr};

				float x{0};
				switch (alignment.horizontal_alignment)
					{
					case utils::alignment::horizontal::left:   x = 0                                          ; break;
					case utils::alignment::horizontal::centre: x = (rect.w() / 2.f) - (element.rect.w() / 2.f); break;
					case utils::alignment::horizontal::right:  x =  rect.w()        -  element.rect.w()       ; break;
					}
				float y{0};
				switch (alignment.vertical_alignment)
					{
					case utils::alignment::vertical::top:    y = 0                                          ; break;
					case utils::alignment::vertical::middle: y = (rect.h() / 2.f) - (element.rect.h() / 2.f); break;
					case utils::alignment::vertical::bottom: y =  rect.h()        -  element.rect.h()       ; break;
					}

				element.reposition({rect.x() + x, rect.y() + y});
				}
			};
		};
	}