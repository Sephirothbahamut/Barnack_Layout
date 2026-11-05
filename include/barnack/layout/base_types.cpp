#include "base_types.h"

namespace barnack::layout
	{
	namespace custom
		{
		size::size(const float _min, const float _max)
			{
			if (_min > _max) 
				{
				throw sizes_error; 
				}
			this->_min = _min;
			this->_max = _max;
			}
		size::size(const float _min, const float _prf, float _max)
			{
			if ((_min > _max) || (_prf < _min) || (_prf > _max)) 
				{
				throw sizes_error; 
				}
			this->_min = _min;
			this->_max = _max;
			this->_prf = _prf;
			}

		void size::set_min(const float val)
			{
			if ((!std::isnan(_prf) && val > _prf) || (val > _max))
				{
				throw sizes_error;
				}
			_min = val;
			_max = std::max(_max, _min);
			}

		void size::set_prf(const float val)
			{
			if (val < _min || val > _max) 
				{
				throw sizes_error; 
				}
			_prf = val;
			}

		void size::set_max(const float& val)
			{
			if ((!std::isnan(_prf) && val < _prf) || (val < _min))
				{
				throw sizes_error;
				}
			_max = val;
			_min = std::min(_min, _max);
			}

		float size::get_prf() const noexcept { return _prf; }
		float size::get_min() const noexcept { return _min; }
		float size::get_max() const noexcept { return _max; }


		sizes::sizes(const size& x, const size& y) : x{x}, y{y} {}
		sizes::sizes(const value_type& min, const value_type& max) :
			x{min.x(), max.x()}, 
			y{min.y(), max.y()} 
			{}
		sizes::sizes(const value_type& min, const value_type& prf, const value_type& max) :
			x{min.x(), prf.x(), max.x()},
			y{min.y(), prf.y(), max.y()}
			{}

		void sizes::set_min(const value_type& val)
			{
			x.set_min(val.x());
			y.set_min(val.y());
			}
		void sizes::set_prf(const value_type& val)
			{
			x.set_prf(val.x());
			y.set_prf(val.y());
			}
		void sizes::set_max(const value_type& val)
			{
			x.set_max(val.x());
			y.set_max(val.y());
			}

		auto sizes::get_prf() const noexcept -> value_type { return {x.prf(), y.prf()}; }
		auto sizes::get_min() const noexcept -> value_type { return {x.min(), y.min()}; }
		auto sizes::get_max() const noexcept -> value_type { return {x.max(), y.max()}; }



		spacing::spacing(const size& ll, const size& up, const size& rr, const size& dw) :
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