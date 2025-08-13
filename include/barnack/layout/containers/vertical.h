#pragma once

#include "common.h"

namespace barnack::layout::containers
	{
	struct vertical : public core::container
		{
		utils::alignment::horizontal alignment{utils::alignment::horizontal::left};

		virtual utils::math::vec2f evaluate_size_min() const noexcept final override
			{
			utils::math::vec2f ret{0.f, 0.f};
			for (const auto& element_ptr : elements)
				{
				utils::math::vec2f element_val{element_ptr->get_size_min()};
				ret.y() += element_val.y();
				ret.x() = std::max(ret.x(), element_val.x());
				}
			return ret;
			}
		virtual utils::math::vec2f evaluate_size_prf() const noexcept final override
			{
			utils::math::vec2f ret{0.f, 0.f};
			for (const auto& element_ptr : elements)
				{
				utils::math::vec2f element_val{element_ptr->get_size_prf()};
				ret.y() += element_val.y();
				ret.x() = std::max(ret.x(), element_val.x());
				}
			return ret;
			}
		virtual utils::math::vec2f evaluate_size_max() const noexcept final override
			{
			utils::math::vec2f ret{0.f, 0.f};
			for (const auto& element_ptr : elements)
				{
				utils::math::vec2f element_val{element_ptr->get_size_max()};
				ret.y() += element_val.y();
				ret.x() = std::max(ret.x(), element_val.x());
				}
			return ret;
			}

		virtual void resize(const utils::math::vec2f& max_size)  final override
			{
			core::element::resize_checked(max_size);

			std::vector<constraints_t> constraints; constraints.reserve(elements.size());
			for (const auto& element_ptr : elements)
				{
				const auto& element{*element_ptr};
				constraints.emplace_back(constraints_t::ver(element));
				}
			auto sizes{calc_sizes({rect.height(), rect.width()}, constraints)};

			for (size_t i{0}; i < elements.size(); i++)
				{
				auto& element{*elements[i]};
				const auto& size{sizes[i]};
				element.resize({size.second, size.first});
				}
			}
		virtual void reposition(const utils::math::vec2f& position) noexcept override
			{
			core::element::reposition(position);

			float y{0};
			for (auto& element_ptr : elements)
				{
				auto& element{*element_ptr};
				float x{0};
				switch (alignment)
					{
					case utils::alignment::horizontal::left:   x = 0                                          ; break;
					case utils::alignment::horizontal::centre: x = (rect.w() / 2.f) - (element.rect.w() / 2.f); break;
					case utils::alignment::horizontal::right:  x =  rect.w()        -  element.rect.w()       ; break;
					}

				element.reposition({rect.x() + x, rect.y() + y});
				y += element_ptr->rect.height();
				}
			};
		};
	}