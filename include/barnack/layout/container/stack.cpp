#include "stack.h"

namespace barnack::layout::container
	{
	utils::math::vec2f stack::evaluate_size_min() const noexcept
		{
		utils::math::vec2f ret{0.f, 0.f};
		for_each_child([&](const layout::element& child)
			{
			utils::math::vec2f child_val{child.get_size_min()};
			ret.x() = std::max(ret.x(), child_val.x());
			ret.y() = std::max(ret.y(), child_val.y());
			});
		return ret;
		}
	utils::math::vec2f stack::evaluate_size_prf() const noexcept
		{
		utils::math::vec2f ret{0.f, 0.f};
		for_each_child([&](const layout::element& child)
			{
			utils::math::vec2f child_val{child.get_size_prf()};
			ret.x() = layout::details::max_nan_aware(ret.x(), child_val.x());
			ret.y() = layout::details::max_nan_aware(ret.y(), child_val.y());
			});
		return ret;
		}
	utils::math::vec2f stack::evaluate_size_max() const noexcept
		{
		utils::math::vec2f ret{0.f, 0.f};
		for_each_child([&](const layout::element& child)
			{
			utils::math::vec2f child_val{child.get_size_max()};
			ret.x() = std::max(ret.x(), child_val.x());
			ret.y() = std::max(ret.y(), child_val.y());
			});
		return ret;
		}

	void stack::resize(const utils::math::vec2f& max_size)
		{
		layout::element::resize_checked(max_size);

		for_each_child([&](layout::element& child)
			{
			child.resize_checked(get_rect_content().size());
			});
		}

	void stack::reposition(const utils::math::vec2f& position) noexcept
		{
		layout::element::reposition(position);

		for_each_slot([&](slot_t& slot)
			{
			auto& child_own{slot.child_own};
			assert(child_own);
			auto& child{*child_own};

			float x{0};
			switch (slot.alignment.horizontal_alignment)
				{
				case utils::alignment::horizontal::left:   x = 0                                                        ; break;
				case utils::alignment::horizontal::centre: x = (get_rect_content().w() / 2.f) - (child.get_rect_content().w() / 2.f); break;
				case utils::alignment::horizontal::right:  x =  get_rect_content().w()        -  child.get_rect_content().w()       ; break;
				}
			float y{0};
			switch (slot.alignment.vertical_alignment)
				{
				case utils::alignment::vertical::top:    y = 0                                                        ; break;
				case utils::alignment::vertical::middle: y = (get_rect_content().h() / 2.f) - (child.get_rect_content().h() / 2.f); break;
				case utils::alignment::vertical::bottom: y =  get_rect_content().h()        -  child.get_rect_content().h()       ; break;
				}

			child.reposition({get_rect_content().x() + x, get_rect_content().y() + y});
			});
		}
	}