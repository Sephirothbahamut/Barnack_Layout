#pragma once

#include <utils/alignment.h>

#include "../core.h"

namespace barnack::layout::wrappers
	{
	struct padding_values
		{
		float up{0};
		float ll{0};
		float dw{0};
		float rr{0};
		utils::math::vec2f tot() const noexcept { return {rr + ll, up + dw}; }
		};

	class Padding : public core::wrapper, public padding_values
		{
		public:
			using init = padding_values;
			Padding(const init& init) : padding_values{init} {}

			virtual utils::math::vec2f evaluate_size_min() const noexcept final override { return element->get_size_min() + tot(); }
			virtual utils::math::vec2f evaluate_size_prf() const noexcept final override { return element->get_size_prf() + tot(); }
			virtual utils::math::vec2f evaluate_size_max() const noexcept final override { return element->get_size_max() + tot(); }

			virtual void resize(const utils::math::vec2f& max_size)  final override
				{
				core::element::resize_checked(max_size);
				element->resize(rect.size() - tot());
				}
			virtual void reposition(utils::math::vec2f position) noexcept
				{
				core::element::reposition(position);
				element->reposition(rect.position() + utils::math::vec2f{ll, up});
				};
		private:
		};

	class Variable_padding : public core::wrapper
		{
		public:
			padding_values min;
			padding_values prf;
			padding_values max;

			virtual utils::math::vec2f evaluate_size_min() const noexcept final override { return element->get_size_min() + min.tot(); }
			virtual utils::math::vec2f evaluate_size_prf() const noexcept final override { return element->get_size_prf() + prf.tot(); }
			virtual utils::math::vec2f evaluate_size_max() const noexcept final override { return element->get_size_max() + max.tot(); }

			//TODO
			virtual void resize(const utils::math::vec2f& max_size)  final override
				{
				core::element::resize_checked(max_size);
				element->resize(rect.size() - min.tot());
				}
			virtual void reposition(const utils::math::vec2f& position) noexcept override
				{
				core::element::reposition(position);
				element->reposition(rect.position() + utils::math::vec2f{min.ll, min.up});
				};

		private:
		};
	}