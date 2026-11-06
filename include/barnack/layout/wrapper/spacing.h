#pragma once

#include "common.h"

namespace barnack::layout::wrapper
	{
	class spacing_fixed : public details::with_slot_t<layout::slot>
		{
		public:
			spacing_fixed() = default;
			spacing_fixed(float value) noexcept;
			spacing_fixed(utils::math::rect_spacing<float> spacing) noexcept;

			utils::math::rect_spacing<float> spacing;

			virtual utils::math::vec2f evaluate_size_min() const noexcept final override;
			virtual utils::math::vec2f evaluate_size_prf() const noexcept final override;
			virtual utils::math::vec2f evaluate_size_max() const noexcept final override;

			virtual void resize    (const utils::math::vec2f& max_size) override;
			virtual void reposition(const utils::math::vec2f& position) noexcept override;
		private:
		};

	//class spacing : public details::with_slot_t<layout::slot>
	//	{
	//	public:
	//		spacing() = default;
	//		spacing(custom::size custom_spacing) noexcept;
	//		spacing(custom::spacing custom_spacing) noexcept;
	//
	//		custom::spacing custom_spacing;
	//
	//		virtual utils::math::vec2f evaluate_size_min() const noexcept final override;
	//		virtual utils::math::vec2f evaluate_size_prf() const noexcept final override;
	//		virtual utils::math::vec2f evaluate_size_max() const noexcept final override;
	//
	//		//TODO
	//		virtual void resize(const utils::math::vec2f& max_size)  final override;
	//		virtual void reposition(const utils::math::vec2f& position) noexcept override;
	//
	//	private:
	//	};
	}

#ifdef barnack_implementation
#include "spacing.cpp"
#endif