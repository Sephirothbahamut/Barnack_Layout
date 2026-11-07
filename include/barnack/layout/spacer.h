#pragma once

#include "element.h"

namespace barnack::layout
	{
	class spacer : public element
		{
		public:
			spacer() = default;
			spacer(constraints::monodirectional constraints) noexcept;
			spacer(constraints::bidirectional   constraints) noexcept;

			constraints::bidirectional constraints;

			virtual utils::math::vec2f evaluate_size_min() const noexcept override;
			virtual utils::math::vec2f evaluate_size_prf() const noexcept override;
			virtual utils::math::vec2f evaluate_size_max() const noexcept override;
		};
	}

#ifdef barnack_implementation
#include "spacer.cpp"
#endif