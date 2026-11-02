#pragma once

#include "common.h"

namespace barnack::layout::container
	{
	struct stack : public details::with_alignment<utils::alignment>
		{
		using parent_t = details::with_alignment<utils::alignment>;
		using parent_t::with_alignment;

		virtual utils::math::vec2f evaluate_size_min() const noexcept final override
			{
			utils::math::vec2f ret{0.f, 0.f};
			for_each_child([&](const core::element& child)
				{
				utils::math::vec2f child_val{child.get_size_min()};
				ret.x() = std::max(ret.x(), child_val.x());
				ret.y() = std::max(ret.y(), child_val.y());
				});
			return ret;
			}
		virtual utils::math::vec2f evaluate_size_prf() const noexcept final override
			{
			utils::math::vec2f ret{0.f, 0.f};
			for_each_child([&](const core::element& child)
				{
				utils::math::vec2f child_val{child.get_size_prf()};
				ret.x() = layout::details::max_nan_aware(ret.x(), child_val.x());
				ret.y() = layout::details::max_nan_aware(ret.y(), child_val.y());
				});
			return ret;
			}
		virtual utils::math::vec2f evaluate_size_max() const noexcept final override
			{
			utils::math::vec2f ret{0.f, 0.f};
			for_each_child([&](const core::element& child)
				{
				utils::math::vec2f child_val{child.get_size_max()};
				ret.x() = std::max(ret.x(), child_val.x());
				ret.y() = std::max(ret.y(), child_val.y());
				});
			return ret;
			}

		virtual void resize(const utils::math::vec2f& max_size)  final override
			{
			core::element::resize_checked(max_size);

			for_each_child([&](core::element& child)
				{
				child.resize_checked(rect_content.size());
				});
			}

		virtual void reposition(const utils::math::vec2f& position) noexcept final override
			{
			core::element::reposition(position);

			for_each_slot([&](slot_t& slot)
				{
				auto& child_own{slot.child_own};
				assert(child_own);
				auto& child{*child_own};

				float x{0};
				switch (slot.alignment.horizontal_alignment)
					{
					case utils::alignment::horizontal::left:   x = 0                                                        ; break;
					case utils::alignment::horizontal::centre: x = (rect_content.w() / 2.f) - (child.rect_content.w() / 2.f); break;
					case utils::alignment::horizontal::right:  x =  rect_content.w()        -  child.rect_content.w()       ; break;
					}
				float y{0};
				switch (slot.alignment.vertical_alignment)
					{
					case utils::alignment::vertical::top:    y = 0                                                        ; break;
					case utils::alignment::vertical::middle: y = (rect_content.h() / 2.f) - (child.rect_content.h() / 2.f); break;
					case utils::alignment::vertical::bottom: y =  rect_content.h()        -  child.rect_content.h()       ; break;
					}

				child.reposition({rect_content.x() + x, rect_content.y() + y});
				});
			}
		};
	}