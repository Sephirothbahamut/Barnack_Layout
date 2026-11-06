#pragma once

#include "common.h"

namespace barnack::layout::wrapper
	{
	class pure : public details::with_slot_t<layout::slot>
		{
		public:
			utils::math::vec2f evaluate_size_min() const noexcept final override;
			utils::math::vec2f evaluate_size_prf() const noexcept final override;
			utils::math::vec2f evaluate_size_max() const noexcept final override;

			virtual void resize    (const utils::math::vec2f& max_size)          override;
			virtual void reposition(const utils::math::vec2f& position) noexcept override;

		private:
		};
	}

#ifdef barnack_implementation
#include "pure.cpp"
#endif