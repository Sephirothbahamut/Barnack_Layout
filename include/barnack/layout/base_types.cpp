#include "base_types.h"

inline static const std::runtime_error sizes_error{"Trying to assign invalid custom sizes. The following must be true for both x and y: _min <= _max."};

namespace barnack::layout
	{
	namespace constraints
		{
		bool monodirectional::unchecked::validate        () const noexcept { return validate        (min, prf, max); }
		bool monodirectional::unchecked::validate_min_max() const noexcept { return validate_min_max(min, max     ); }
		bool monodirectional::unchecked::validate_min_prf() const noexcept { return validate_min_prf(min, prf     ); }
		bool monodirectional::unchecked::validate_max_prf() const noexcept { return validate_max_prf(max, prf     ); }

		bool monodirectional::unchecked::validate(const float min, const float prf, float max) noexcept
			{
			return validate_min_max(min, max) && validate_min_prf(min, prf) && validate_max_prf(max, prf);
			}
		bool monodirectional::unchecked::validate_min_max(float min, float max) noexcept
			{
			const bool both_valid{!std::isnan(min) && !std::isnan(max)};
			return !(both_valid && (min > max));
			}
		bool monodirectional::unchecked::validate_min_prf(float min, float prf) noexcept
			{
			const bool both_valid{!std::isnan(min) && !std::isnan(prf)};
			return !(both_valid && (min > prf));
			}
		bool monodirectional::unchecked::validate_max_prf(float max, float prf) noexcept
			{
			const bool both_valid{!std::isnan(max) && !std::isnan(prf)};
			return !(both_valid && (prf > max));
			}

		monodirectional::monodirectional(const float _min, const float _max) : values{.min{_min}, .max{_max}}
			{
			if (!values.validate_min_max())
				{
				throw sizes_error; 
				}
			}
		monodirectional::monodirectional(const float _min, const float _prf, float _max) : values{.min{_min}, .prf{_prf}, .max{_max}}
			{
			if (!values.validate())
				{
				throw sizes_error;
				}
			}

		void monodirectional::set_min(const float val)
			{
			if (!unchecked::validate_min_max(val, values.max) || !unchecked::validate_min_prf(val, values.prf))
				{
				throw sizes_error;
				}
			values.min = val;
			}

		void monodirectional::set_prf(const float val)
			{
			if (!unchecked::validate_min_prf(values.min, val) || !unchecked::validate_max_prf(values.max, val))
				{
				throw sizes_error;
				}
			values.prf = val;
			}

		void monodirectional::set_max(const float& val)
			{
			if (!unchecked::validate_min_max(values.min, val) || !unchecked::validate_max_prf(val, values.prf))
				{
				throw sizes_error;
				}
			values.max = val;
			}

		float monodirectional::get_min() const noexcept { return values.min; }
		float monodirectional::get_prf() const noexcept { return values.prf; }
		float monodirectional::get_max() const noexcept { return values.max; }

		bidirectional::bidirectional(const monodirectional& x, const monodirectional& y) : x{x}, y{y} {}
		bidirectional::bidirectional(const value_type& min, const value_type& max) :
			x{min.x(), max.x()}, 
			y{min.y(), max.y()} 
			{}
		bidirectional::bidirectional(const value_type& min, const value_type& prf, const value_type& max) :
			x{min.x(), prf.x(), max.x()},
			y{min.y(), prf.y(), max.y()}
			{}

		void bidirectional::set_min(const value_type& val)
			{
			x.set_min(val.x());
			y.set_min(val.y());
			}
		void bidirectional::set_prf(const value_type& val)
			{
			x.set_prf(val.x());
			y.set_prf(val.y());
			}
		void bidirectional::set_max(const value_type& val)
			{
			x.set_max(val.x());
			y.set_max(val.y());
			}

		auto bidirectional::get_prf() const noexcept -> value_type { return {x.prf(), y.prf()}; }
		auto bidirectional::get_min() const noexcept -> value_type { return {x.min(), y.min()}; }
		auto bidirectional::get_max() const noexcept -> value_type { return {x.max(), y.max()}; }



		spacing::spacing(const monodirectional& ll, const monodirectional& up, const monodirectional& rr, const monodirectional& dw) :
			ll{ll},
			up{up},
			rr{rr},
			dw{dw}
			{}
		spacing::spacing(const value_type& min, const value_type& max) :
			ll{min.ll(), max.ll()},
			up{min.up(), max.up()},
			rr{min.rr(), max.rr()},
			dw{min.dw(), max.dw()}
			{}
		spacing::spacing(const value_type& min, const value_type& prf, const value_type& max) :
			ll{min.ll(), prf.ll(), max.ll()},
			up{min.up(), prf.up(), max.up()},
			rr{min.rr(), prf.rr(), max.rr()},
			dw{min.dw(), prf.dw(), max.dw()}
			{}

		void spacing::set_min(const value_type& val)
			{
			ll.set_min(val.ll());
			up.set_min(val.up());
			rr.set_min(val.rr());
			dw.set_min(val.dw());
			}
		void spacing::set_prf(const value_type& val)
			{
			ll.set_prf(val.ll());
			up.set_prf(val.up());
			rr.set_prf(val.rr());
			dw.set_prf(val.dw());
			}
		void spacing::set_max(const value_type& val)
			{
			ll.set_max(val.ll());
			up.set_max(val.up());
			rr.set_max(val.rr());
			dw.set_max(val.dw());
			}

		auto spacing::get_prf() const noexcept -> value_type { return {ll.prf(), up.prf(), rr.prf(), dw.prf()}; }
		auto spacing::get_min() const noexcept -> value_type { return {ll.min(), up.min(), rr.min(), dw.min()}; }
		auto spacing::get_max() const noexcept -> value_type { return {ll.max(), up.max(), rr.max(), dw.max()}; }
		}

	namespace details
		{
		float filter_nan(float a, float b) noexcept
			{
			if (std::isnan(a)) { return b; }
			return a;
			}
		float add_nan_aware(float a, float b) noexcept
			{
			if (std::isnan(a)) { return b; }
			else if (std::isnan(b)) { return a; }
			return a + b;
			}
		float max_nan_aware(float a, float b) noexcept
			{
			if (std::isnan(a)) { return b; }
			else if (std::isnan(b)) { return a; }
			return std::max(a, b);
			}
		float min_nan_aware(float a, float b) noexcept
			{
			if (std::isnan(a)) { return b; }
			else if (std::isnan(b)) { return a; }
			return std::min(a, b);
			}
		float avg_nan_aware(float a, float b) noexcept
			{
			if (std::isnan(a)) { return b; }
			else if (std::isnan(b)) { return a; }
			return (a + b) / 2.f;
			}

		utils::math::vec2f filter_nan(const utils::math::vec2f& a, const utils::math::vec2f& b) noexcept
			{
			return a.operator_to_new(b, [](const auto& a, const auto& b) { return layout::details::filter_nan(a, b); });;
			}
		utils::math::vec2f add_nan_aware(const utils::math::vec2f& a, const utils::math::vec2f& b) noexcept
			{
			return a.operator_to_new(b, [](const auto& a, const auto& b) { return layout::details::add_nan_aware(a, b); });;
			}
		utils::math::vec2f max_nan_aware(const utils::math::vec2f& a, const utils::math::vec2f& b) noexcept
			{
			return a.operator_to_new(b, [](const auto& a, const auto& b) { return layout::details::max_nan_aware(a, b); });;
			}
		utils::math::vec2f min_nan_aware(const utils::math::vec2f& a, const utils::math::vec2f& b) noexcept
			{
			return a.operator_to_new(b, [](const auto& a, const auto& b) { return layout::details::min_nan_aware(a, b); });;
			}
		utils::math::vec2f avg_nan_aware(const utils::math::vec2f& a, const utils::math::vec2f& b) noexcept
			{
			return a.operator_to_new(b, [](const auto& a, const auto& b) { return layout::details::avg_nan_aware(a, b); });;
			}




		//Note: std::clamp throws/UBs if min > max
		//Instead here we want to favour one
		float clamp_favour_min(float value, float min, float max) noexcept
			{
			return std::max(std::min(value, max), min);
			}

		//Note: std::clamp throws/UBs if min > max
		//Instead here we want to favour one
		float clamp_favour_min_nan_aware(float value, float min, float max) noexcept
			{
			return max_nan_aware(min_nan_aware(value, max), min);
			}



		float evaluate_element_position_for_alignment_based_container
			(
			const utils::math::rect<float>& container_rect,
			const utils::math::rect<float>& element_rect,
			const utils::alignment::horizontal& alignment
			) noexcept
			{
			switch (alignment)
				{
				case utils::alignment::horizontal::left:   return 0                                                    ;
				case utils::alignment::horizontal::centre: return (container_rect.w() / 2.f) - (element_rect.w() / 2.f);
				case utils::alignment::horizontal::right:  return  container_rect.w()        -  element_rect.w()       ;
				}
			std::unreachable();
			}

		float evaluate_element_position_for_alignment_based_container
			(
			const utils::math::rect<float>& container_rect,
			const utils::math::rect<float>& element_rect,
			const utils::alignment::vertical& alignment
			) noexcept
			{
			switch (alignment)
				{
				case utils::alignment::vertical::top:    return 0                                                    ;
				case utils::alignment::vertical::middle: return (container_rect.h() / 2.f) - (element_rect.h() / 2.f);
				case utils::alignment::vertical::bottom: return  container_rect.h()        -  element_rect.h()       ;
				}
			std::unreachable();
			}
		}
	}