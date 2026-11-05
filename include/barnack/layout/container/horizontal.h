#pragma once

#include "common.h"

namespace barnack::layout::container
	{
	struct horizontal : public details::with_alignment<utils::alignment::vertical>
		{
		using parent_t = details::with_alignment<utils::alignment::vertical>;
		using parent_t::with_alignment;

		virtual utils::math::vec2f evaluate_size_min() const noexcept final override;
		virtual utils::math::vec2f evaluate_size_prf() const noexcept final override;
		virtual utils::math::vec2f evaluate_size_max() const noexcept final override;

		virtual void resize    (const utils::math::vec2f& max_size)          final override;
		virtual void reposition(const utils::math::vec2f& position) noexcept final override;
		};
	}

#ifdef barnack_layout_implementation
#include "horizontal.cpp"
#endif