#pragma once

#include "common.h"

namespace barnack::layout::containers
	{
	struct horizontal : public core::container
		{
		utils::alignment::vertical alignment{utils::alignment::vertical::top};

		virtual utils::math::vec2f evaluate_size_min() const noexcept final override
			{
			utils::math::vec2f ret{0.f, 0.f};
			for (const auto& element_ptr : elements)
				{
				utils::math::vec2f element_val{element_ptr->get_size_min()};
				ret.x() += element_val.x();
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
				ret.x() += element_val.x();
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
				ret.x() += element_val.x();
				ret.y() = std::max(ret.y(), element_val.y());
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
				constraints.emplace_back(constraints_t::hor(element));
				}
			auto sizes{calc_sizes({rect.width(), rect.height()}, constraints)};

			for (size_t i{0}; i < elements.size(); i++)
				{
				auto& element{*elements[i]};
				const auto& size{sizes[i]};
				element.resize({size.first, size.second});
				}
			}
		virtual void reposition(const utils::math::vec2f& position) noexcept override
			{
			core::element::reposition(position);

			float x{0};
			for (auto& element_ptr : elements)
				{
				auto& element{*element_ptr};
				float y{0};
				switch (alignment)
					{
					case utils::alignment::vertical::top:    y = 0                                          ; break;
					case utils::alignment::vertical::middle: y = (rect.h() / 2.f) - (element.rect.h() / 2.f); break;
					case utils::alignment::vertical::bottom: y =  rect.h()        -  element.rect.h()       ; break;
					}

				element.reposition({rect.x() + x, rect.y() + y});
				x += element_ptr->rect.width();
				}
			};
		};
	}