#pragma once

#include <cmath>

#include <utils/math/vec.h>
#include <utils/math/rect.h>
#include <utils/math/constants.h>
#include <utils/oop/proxy.h>

namespace barnack::layout
	{
	class custom_sizes_t
		{
		public:
			custom_sizes_t() = default;
			custom_sizes_t(const utils::math::vec2f& _min, const utils::math::vec2f& _max)
				{
				if (_min.x() > _max.x() || _min.y() > _max.y()) { throw sizes_error; }
				this->_min = _min;
				this->_max = _max;
				}
			custom_sizes_t(const utils::math::vec2f& _min, const utils::math::vec2f& _prf, const utils::math::vec2f& _max)
				{
				if (_min.x() > _max.x() || _min.y() > _max.y() || _prf.x() < _min.x() || _prf.x() > _max.x() || _prf.y() < _min.y() || _prf.y() > _max.y()) { throw sizes_error; }
				this->_min = _min;
				this->_max = _max;
				this->_prf = _prf;
				}

#pragma region Setters
			void set_min(const utils::math::vec2f& val)
				{
				if ((!std::isnan(_prf.x()) && val.x() > _prf.x()) ||
					(!std::isnan(_prf.y()) && val.y() > _prf.y()) ||
					val.x() > _max.x() || val.y() > _max.y())
					{
					throw sizes_error;
					}
				_min = val;
				}
			void set_prf(const utils::math::vec2f& val)
				{
				if (val.x() < _min.x() || val.x() > _max.x() || val.y() < _min.y() || val.y() > _max.y()) { throw sizes_error; }
				_prf = val;
				}
			void set_max(const utils::math::vec2f& val)
				{
				if ((!std::isnan(_prf.x()) && val.x() < _prf.x()) ||
					(!std::isnan(_prf.y()) && val.y() < _prf.y()) ||
					val.x() < _min.x() || val.y() < _min.y())
					{
					throw sizes_error;
					}
				_max = val;
				}
			void set_min_x(float x)
				{
				if (!std::isnan(_prf.x())) { if (x > _prf.x()) { throw sizes_error; } }
				else { if (x > _max.x()) { throw sizes_error; } }
				_min.x() = x;
				}
			void set_min_y(float y)
				{
				if (!std::isnan(_prf.y())) { if (y > _prf.y()) { throw sizes_error; } }
				else { if (y > _max.y()) { throw sizes_error; } }
				_min.y() = y;
				}
			void set_prf_x(float x)
				{
				if (x < _min.x() || x > _max.x()) { throw sizes_error; }
				_prf.x() = x;
				}
			void set_prf_y(float y)
				{
				if (y < _min.y() || y > _max.y()) { throw sizes_error; }
				_prf.y() = y;
				}
			void set_max_x(float x)
				{
				if (!std::isnan(_prf.x())) { if (x < _prf.x()) { throw sizes_error; } }
				else { if (x < _min.x()) { throw sizes_error; } }
				_max.x() = x;
				}
			void set_max_y(float y)
				{
				if (!std::isnan(_prf.y())) { if (y < _prf.y()) { throw sizes_error; } }
				else { if (y < _min.y()) { throw sizes_error; } }
				_max.y() = y;
				}
#pragma endregion Setters
#pragma region Getters
			utils::math::vec2f get_prf  () const noexcept { return _prf; }
			utils::math::vec2f get_min  () const noexcept { return _min; }
			utils::math::vec2f get_max  () const noexcept { return _max; }
			float              get_min_x() const noexcept { return _min.x(); }
			float              get_min_y() const noexcept { return _min.y(); }
			float              get_prf_x() const noexcept { return _prf.x(); }
			float              get_prf_y() const noexcept { return _prf.y(); }
			float              get_max_x() const noexcept { return _max.x(); }
			float              get_max_y() const noexcept { return _max.y(); }
#pragma endregion Getters

#pragma region Properties
				
			auto min  () const noexcept { return get_min  (); }
			auto prf  () const noexcept { return get_prf  (); }
			auto max  () const noexcept { return get_max  (); }

			auto min_x() const noexcept { return get_min_x(); }
			auto min_y() const noexcept { return get_min_x(); }
			auto prf_x() const noexcept { return get_prf_x(); }
			auto prf_y() const noexcept { return get_prf_x(); }
			auto max_x() const noexcept { return get_max_x(); }
			auto max_y() const noexcept { return get_max_x(); }

			auto proxy_min  ()       noexcept { return utils::oop::proxy<custom_sizes_t, false, utils::math::vec2f, &custom_sizes_t::set_min  , &custom_sizes_t::get_min  >{*this}; }
			auto proxy_min  () const noexcept { return utils::oop::proxy<custom_sizes_t, true , utils::math::vec2f, &custom_sizes_t::set_min  , &custom_sizes_t::get_min  >{*this}; }
			auto proxy_prf  ()       noexcept { return utils::oop::proxy<custom_sizes_t, false, utils::math::vec2f, &custom_sizes_t::set_prf  , &custom_sizes_t::get_prf  >{*this}; }
			auto proxy_prf  () const noexcept { return utils::oop::proxy<custom_sizes_t, true , utils::math::vec2f, &custom_sizes_t::set_prf  , &custom_sizes_t::get_prf  >{*this}; }
			auto proxy_max  ()       noexcept { return utils::oop::proxy<custom_sizes_t, false, utils::math::vec2f, &custom_sizes_t::set_max  , &custom_sizes_t::get_max  >{*this}; }
			auto proxy_max  () const noexcept { return utils::oop::proxy<custom_sizes_t, true , utils::math::vec2f, &custom_sizes_t::set_max  , &custom_sizes_t::get_max  >{*this}; }
			
			auto proxy_min_x()       noexcept { return utils::oop::proxy<custom_sizes_t, false, float             , &custom_sizes_t::set_min_x, &custom_sizes_t::get_min_x>{*this}; }
			auto proxy_min_x() const noexcept { return utils::oop::proxy<custom_sizes_t, true , float             , &custom_sizes_t::set_min_x, &custom_sizes_t::get_min_x>{*this}; }
			auto proxy_min_y()       noexcept { return utils::oop::proxy<custom_sizes_t, false, float             , &custom_sizes_t::set_min_y, &custom_sizes_t::get_min_x>{*this}; }
			auto proxy_min_y() const noexcept { return utils::oop::proxy<custom_sizes_t, true , float             , &custom_sizes_t::set_min_y, &custom_sizes_t::get_min_x>{*this}; }
			auto proxy_prf_x()       noexcept { return utils::oop::proxy<custom_sizes_t, false, float             , &custom_sizes_t::set_prf_x, &custom_sizes_t::get_prf_x>{*this}; }
			auto proxy_prf_x() const noexcept { return utils::oop::proxy<custom_sizes_t, true , float             , &custom_sizes_t::set_prf_x, &custom_sizes_t::get_prf_x>{*this}; }
			auto proxy_prf_y()       noexcept { return utils::oop::proxy<custom_sizes_t, false, float             , &custom_sizes_t::set_prf_y, &custom_sizes_t::get_prf_x>{*this}; }
			auto proxy_prf_y() const noexcept { return utils::oop::proxy<custom_sizes_t, true , float             , &custom_sizes_t::set_prf_y, &custom_sizes_t::get_prf_x>{*this}; }
			auto proxy_max_x()       noexcept { return utils::oop::proxy<custom_sizes_t, false, float             , &custom_sizes_t::set_max_x, &custom_sizes_t::get_max_x>{*this}; }
			auto proxy_max_x() const noexcept { return utils::oop::proxy<custom_sizes_t, true , float             , &custom_sizes_t::set_max_x, &custom_sizes_t::get_max_x>{*this}; }
			auto proxy_max_y()       noexcept { return utils::oop::proxy<custom_sizes_t, false, float             , &custom_sizes_t::set_max_y, &custom_sizes_t::get_max_x>{*this}; }
			auto proxy_max_y() const noexcept { return utils::oop::proxy<custom_sizes_t, true , float             , &custom_sizes_t::set_max_y, &custom_sizes_t::get_max_x>{*this}; }

//			__declspec(property(get = get_min, put = set_min)) utils::math::vec2f min;
//			__declspec(property(get = get_prf, put = set_prf)) utils::math::vec2f prf;
//			__declspec(property(get = get_max, put = set_max)) utils::math::vec2f max;
//
//			__declspec(property(put = set_min_x)) float min_x;
//			__declspec(property(put = set_min_y)) float min_y;
//			__declspec(property(put = set_prf_x)) float prf_x;
//			__declspec(property(put = set_prf_y)) float prf_y;
//			__declspec(property(put = set_max_x)) float max_x;
//			__declspec(property(put = set_max_y)) float max_y;
#pragma endregion Properties

		private:
			inline static const std::runtime_error sizes_error{"Trying to assign invalid custom sizes. The following must be true for both x and y: _min <= _max."};

			utils::math::vec2f _min{0.f, 0.f}; //can be [0, _max]
			utils::math::vec2f _max{utils::math::constants::finf, utils::math::constants::finf}; //can be [_min, finf]
			utils::math::vec2f _prf{utils::math::constants::fnan, utils::math::constants::fnan}; //can be [_min, _max] or fnan
		};
#pragma endregion inner_types
	}