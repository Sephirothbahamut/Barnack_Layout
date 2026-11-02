#pragma once

#include <cmath>

#include <utils/math/vec.h>
#include <utils/math/rect.h>
#include <utils/math/constants.h>
#include <utils/oop/proxy.h>

namespace barnack::layout
	{
	namespace custom
		{
		class size
			{
			public:
				size() = default;
				size(const float _min, const float _max)
					{
					if (_min > _max) 
						{
						throw sizes_error; 
						}
					this->_min = _min;
					this->_max = _max;
					}
				size(const float _min, const float _prf, float _max)
					{
					if ((_min > _max) || (_prf < _min) || (_prf > _max)) 
						{
						throw sizes_error; 
						}
					this->_min = _min;
					this->_max = _max;
					this->_prf = _prf;
					}

				/// <summary> Must be [0.f, max()]</summary>
				void set_min(const float val)
					{
					if ((!std::isnan(_prf) && val > _prf) || (val > _max))
						{
						throw sizes_error;
						}
					_min = val;
					_max = std::max(_max, _min);
					}
				/// <summary> Must be [min(), max()]</summary>
				void set_prf(const float val)
					{
					if (val < _min || val > _max) 
						{
						throw sizes_error; 
						}
					_prf = val;
					}
				/// <summary> Must be [min(), finf]</summary>
				void set_max(const float& val)
					{
					if ((!std::isnan(_prf) && val < _prf) || (val < _min))
						{
						throw sizes_error;
						}
					_max = val;
					_min = std::min(_min, _max);
					}

				float get_prf  () const noexcept { return _prf; }
				float get_min  () const noexcept { return _min; }
				float get_max  () const noexcept { return _max; }

				auto min()       noexcept { return utils::oop::proxy<size, false, float, &size::set_min, &size::get_min>{*this}; }
				auto min() const noexcept { return utils::oop::proxy<size, true , float, &size::set_min, &size::get_min>{*this}; }
				auto prf()       noexcept { return utils::oop::proxy<size, false, float, &size::set_prf, &size::get_prf>{*this}; }
				auto prf() const noexcept { return utils::oop::proxy<size, true , float, &size::set_prf, &size::get_prf>{*this}; }
				auto max()       noexcept { return utils::oop::proxy<size, false, float, &size::set_max, &size::get_max>{*this}; }
				auto max() const noexcept { return utils::oop::proxy<size, true , float, &size::set_max, &size::get_max>{*this}; }

			private:
				inline static const std::runtime_error sizes_error{"Trying to assign invalid custom sizes. The following must be true for both x and y: _min <= _max."};

				float _min{0.f                         }; //can be [0, _max]
				float _max{utils::math::constants::finf}; //can be [_min, finf]
				float _prf{utils::math::constants::fnan}; //can be [_min, _max] or fnan
			};

		class sizes
			{
			public:
				using value_type = utils::math::vec2f;
				size x;
				size y;

				sizes() = default;
				sizes(const size& x, const size& y) : x{x}, y{y} {}
				sizes(const value_type& min, const value_type& max) : 
					x{min.x(), max.x()}, 
					y{min.y(), max.y()} 
					{}
				sizes(const value_type& min, const value_type& prf, const value_type& max) :
					x{min.x(), prf.x(), max.x()},
					y{min.y(), prf.y(), max.y()}
					{}

				void set_min(const value_type& val)
					{
					x.set_min(val.x());
					y.set_min(val.y());
					}
				void set_prf(const value_type& val)
					{
					x.set_prf(val.x());
					y.set_prf(val.y());
					}
				void set_max(const value_type& val)
					{
					x.set_max(val.x());
					y.set_max(val.y());
					}

				value_type get_prf  () const noexcept { return {x.prf(), y.prf()}; }
				value_type get_min  () const noexcept { return {x.min(), y.min()}; }
				value_type get_max  () const noexcept { return {x.max(), y.max()}; }

				auto min()       noexcept { return utils::oop::proxy<sizes, false, value_type, &sizes::set_min, &sizes::get_min>{*this}; }
				auto min() const noexcept { return utils::oop::proxy<sizes, true , value_type, &sizes::set_min, &sizes::get_min>{*this}; }
				auto prf()       noexcept { return utils::oop::proxy<sizes, false, value_type, &sizes::set_prf, &sizes::get_prf>{*this}; }
				auto prf() const noexcept { return utils::oop::proxy<sizes, true , value_type, &sizes::set_prf, &sizes::get_prf>{*this}; }
				auto max()       noexcept { return utils::oop::proxy<sizes, false, value_type, &sizes::set_max, &sizes::get_max>{*this}; }
				auto max() const noexcept { return utils::oop::proxy<sizes, true , value_type, &sizes::set_max, &sizes::get_max>{*this}; }
			};
	
		class spacing
			{
			public:
				using value_type = utils::math::rect_spacing<float>;

				size ll;
				size up;
				size rr;
				size dw;

				spacing() = default;
				spacing(const size& ll, const size& up, const size& rr, const size& dw) :
					ll{ll},
					up{up},
					rr{rr},
					dw{dw}
					{}
				spacing(const value_type& min, const value_type& max) :
					ll{min.ll(), max.ll()},
					up{min.up(), max.up()},
					rr{min.rr(), max.rr()},
					dw{min.dw(), max.dw()}
					{}
				spacing(const value_type& min, const value_type& prf, const value_type& max) :
					ll{min.ll(), prf.ll(), max.ll()},
					up{min.up(), prf.up(), max.up()},
					rr{min.rr(), prf.rr(), max.rr()},
					dw{min.dw(), prf.dw(), max.dw()}
					{}

				void set_min(const value_type& val)
					{
					ll.set_min(val.ll());
					up.set_min(val.up());
					rr.set_min(val.rr());
					dw.set_min(val.dw());
					}
				void set_prf(const value_type& val)
					{
					ll.set_prf(val.ll());
					up.set_prf(val.up());
					rr.set_prf(val.rr());
					dw.set_prf(val.dw());
					}
				void set_max(const value_type& val)
					{
					ll.set_max(val.ll());
					up.set_max(val.up());
					rr.set_max(val.rr());
					dw.set_max(val.dw());
					}

				value_type get_prf  () const noexcept { return {ll.prf(), up.prf(), rr.prf(), dw.prf()}; }
				value_type get_min  () const noexcept { return {ll.min(), up.min(), rr.min(), dw.min()}; }
				value_type get_max  () const noexcept { return {ll.max(), up.max(), rr.max(), dw.max()}; }

				auto min()       noexcept { return utils::oop::proxy<spacing, false, value_type, &spacing::set_min, &spacing::get_min>{*this}; }
				auto min() const noexcept { return utils::oop::proxy<spacing, true , value_type, &spacing::set_min, &spacing::get_min>{*this}; }
				auto prf()       noexcept { return utils::oop::proxy<spacing, false, value_type, &spacing::set_prf, &spacing::get_prf>{*this}; }
				auto prf() const noexcept { return utils::oop::proxy<spacing, true , value_type, &spacing::set_prf, &spacing::get_prf>{*this}; }
				auto max()       noexcept { return utils::oop::proxy<spacing, false, value_type, &spacing::set_max, &spacing::get_max>{*this}; }
				auto max() const noexcept { return utils::oop::proxy<spacing, true , value_type, &spacing::set_max, &spacing::get_max>{*this}; }

			};
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
			)
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
			)
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