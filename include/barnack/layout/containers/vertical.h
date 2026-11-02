#pragma once

#include "common.h"

namespace barnack::layout::container
	{
	struct vertical : public details::with_alignment<utils::alignment::horizontal>
		{
		using parent_t = details::with_alignment<utils::alignment::horizontal>;
		using parent_t::with_alignment;

		virtual utils::math::vec2f evaluate_size_min() const noexcept final override
			{
			utils::math::vec2f ret{0.f, 0.f};
			for_each_child([&](const core::element& child)
				{
				utils::math::vec2f child_val{child.get_size_min()};
				ret.y() += child_val.y();
				ret.x() = std::max(ret.x(), child_val.x());
				});

			return ret;
			}
		virtual utils::math::vec2f evaluate_size_prf() const noexcept final override
			{
			utils::math::vec2f ret{utils::math::constants::fnan, utils::math::constants::fnan};
			for_each_child([&](const core::element& child)
				{
				utils::math::vec2f child_val{child.get_size_prf()};
				ret.y() = layout::details::add_nan_aware(ret.y(), child_val.y());
				ret.x() = layout::details::max_nan_aware(ret.x(), child_val.x());
				});
			return ret;
			}
		virtual utils::math::vec2f evaluate_size_max() const noexcept final override
			{
			if (slots.empty()) { return {utils::math::constants::finf, utils::math::constants::finf}; }

			utils::math::vec2f ret{0.f, 0.f};
			for_each_child([&](const core::element& child)
				{
				utils::math::vec2f child_val{child.get_size_max()};
				ret.y() += child_val.y();
				ret.x() = std::max(ret.x(), child_val.x());
				});
			return ret;
			}

		virtual void resize(const utils::math::vec2f& max_size)  final override
			{
			core::element::resize_checked(max_size);

			std::vector<constraints_t> constraints(slots.size());
			for (size_t i{0}; i < slots.size(); i++)
				{
				auto& child_own{slots[i].child_own};
				assert(child_own);
				auto& child{*child_own};
				constraints[i] = constraints_t::ver(child);
				}
			auto sizes{details::calc_sizes({rect_content.height(), rect_content.width()}, constraints)};

			for (size_t i{0}; i < slots.size(); i++)
				{
				auto& child_own{slots[i].child_own};
				assert(child_own);
				auto& child{*child_own};
				const auto& size{sizes[i]};
				child.resize({size.second, size.first});
				}
			}
		virtual void reposition(const utils::math::vec2f& position) noexcept final override
			{
			core::element::reposition(position);

			float delta_y{0};
			for_each_slot([&](const slot_t& slot)
				{
				auto& child_own{slot.child_own};
				assert(child_own);
				auto& child{*child_own};

				const float delta_x{layout::details::evaluate_element_position_for_alignment_based_container(rect_content, child.rect_content, slot.alignment)};

				const utils::math::vec2f element_new_position{rect_content.x() + delta_x, rect_content.y() + delta_y};

				child.reposition(element_new_position);
				delta_y += child.rect_content.height();
				});
			};
		};
	}