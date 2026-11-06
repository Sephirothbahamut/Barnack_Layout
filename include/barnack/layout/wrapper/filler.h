#pragma once

#include "common.h"

namespace barnack::layout::wrapper
	{
	struct filler_horizontal : public details::with_alignment<utils::alignment::horizontal>
		{
		virtual utils::math::vec2f evaluate_size_min() const noexcept final override;
		virtual utils::math::vec2f evaluate_size_prf() const noexcept final override;
		virtual utils::math::vec2f evaluate_size_max() const noexcept final override;

		virtual void resize(const utils::math::vec2f& max_size) final override;
		virtual void reposition(const utils::math::vec2f& position) noexcept final override;
		};

	struct filler_vertical : public details::with_alignment<utils::alignment::vertical>
		{
		virtual utils::math::vec2f evaluate_size_min() const noexcept final override;
		virtual utils::math::vec2f evaluate_size_prf() const noexcept final override;
		virtual utils::math::vec2f evaluate_size_max() const noexcept final override;

		virtual void resize(const utils::math::vec2f& max_size) final override;
		virtual void reposition(const utils::math::vec2f& position) noexcept final override;
		};

	struct filler_bidirectional : public details::with_alignment<utils::alignment>
		{
		virtual utils::math::vec2f evaluate_size_min() const noexcept final override;
		virtual utils::math::vec2f evaluate_size_prf() const noexcept final override;
		virtual utils::math::vec2f evaluate_size_max() const noexcept final override;

		virtual void resize(const utils::math::vec2f& max_size) final override;

		virtual void reposition(const utils::math::vec2f& position) noexcept final override;
		};
	}

#ifdef barnack_implementation
#include "filler.cpp"
#endif