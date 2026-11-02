#pragma once

#include "common.h"

namespace barnack::layout::wrapper
	{
	class spacing_fixed : public details::with_slot_t<core::slot>
		{
		public:
			utils::math::rect_spacing<float> spacing;

			virtual utils::math::vec2f evaluate_size_min() const noexcept final override { return child().get_size_min() + spacing; }
			virtual utils::math::vec2f evaluate_size_prf() const noexcept final override { return child().get_size_prf() + spacing; }
			virtual utils::math::vec2f evaluate_size_max() const noexcept final override { return child().get_size_max() + spacing; }

			virtual void resize(const utils::math::vec2f& max_size)  final override
				{
				core::element::resize_checked(max_size);
				child().resize(rect_content.size() - spacing);
				}
			virtual void reposition(utils::math::vec2f position) noexcept
				{
				core::element::reposition(position);
				child().reposition(rect_content.position() + utils::math::vec2f{spacing.ll(), spacing.up()});
				};
		private:
		};

	class spacing : public details::with_slot_t<core::slot>
		{
		public:
			custom::spacing custom_spacing;

			virtual utils::math::vec2f evaluate_size_min() const noexcept final override { return child().get_size_min() + custom_spacing.get_min(); }
			virtual utils::math::vec2f evaluate_size_prf() const noexcept final override { return child().get_size_prf() + custom_spacing.get_prf(); }
			virtual utils::math::vec2f evaluate_size_max() const noexcept final override { return child().get_size_max() + custom_spacing.get_max(); }

			//TODO
			virtual void resize(const utils::math::vec2f& max_size)  final override
				{
				core::element::resize_checked(max_size);
				child().resize(rect_content.size() - custom_spacing.get_min());
				}
			virtual void reposition(const utils::math::vec2f& position) noexcept override
				{
				core::element::reposition(position);
				child().reposition(rect_content.position() + utils::math::vec2f{custom_spacing.ll.min(), custom_spacing.up.min()});
				};

		private:
		};
	}