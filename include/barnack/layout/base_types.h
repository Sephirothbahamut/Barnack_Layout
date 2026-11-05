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
				size(const float _min, const float _max);
				size(const float _min, const float _prf, float _max);

				/// <summary> Must be [0.f, max()]</summary>
				void set_min(const float val);
				/// <summary> Must be [min(), max()]</summary>
				void set_prf(const float val);
				/// <summary> Must be [min(), finf]</summary>
				void set_max(const float& val);

				float get_prf() const noexcept;
				float get_min() const noexcept;
				float get_max() const noexcept;

				inline auto min()       noexcept { return utils::oop::proxy<size, false, float, &size::set_min, &size::get_min>{*this}; }
				inline auto min() const noexcept { return utils::oop::proxy<size, true , float, &size::set_min, &size::get_min>{*this}; }
				inline auto prf()       noexcept { return utils::oop::proxy<size, false, float, &size::set_prf, &size::get_prf>{*this}; }
				inline auto prf() const noexcept { return utils::oop::proxy<size, true , float, &size::set_prf, &size::get_prf>{*this}; }
				inline auto max()       noexcept { return utils::oop::proxy<size, false, float, &size::set_max, &size::get_max>{*this}; }
				inline auto max() const noexcept { return utils::oop::proxy<size, true , float, &size::set_max, &size::get_max>{*this}; }

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
				sizes(const size& x, const size& y);
				sizes(const value_type& min, const value_type& max);
				sizes(const value_type& min, const value_type& prf, const value_type& max);

				void set_min(const value_type& val);
				void set_prf(const value_type& val);
				void set_max(const value_type& val);

				value_type get_prf() const noexcept;
				value_type get_min() const noexcept;
				value_type get_max() const noexcept;

				inline auto min()       noexcept { return utils::oop::proxy<sizes, false, value_type, &sizes::set_min, &sizes::get_min>{*this}; }
				inline auto min() const noexcept { return utils::oop::proxy<sizes, true , value_type, &sizes::set_min, &sizes::get_min>{*this}; }
				inline auto prf()       noexcept { return utils::oop::proxy<sizes, false, value_type, &sizes::set_prf, &sizes::get_prf>{*this}; }
				inline auto prf() const noexcept { return utils::oop::proxy<sizes, true , value_type, &sizes::set_prf, &sizes::get_prf>{*this}; }
				inline auto max()       noexcept { return utils::oop::proxy<sizes, false, value_type, &sizes::set_max, &sizes::get_max>{*this}; }
				inline auto max() const noexcept { return utils::oop::proxy<sizes, true , value_type, &sizes::set_max, &sizes::get_max>{*this}; }
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
				spacing(const size& ll, const size& up, const size& rr, const size& dw);
				spacing(const value_type& min, const value_type& max);
				spacing(const value_type& min, const value_type& prf, const value_type& max);

				void set_min(const value_type& val);
				void set_prf(const value_type& val);
				void set_max(const value_type& val);

				value_type get_prf() const noexcept;
				value_type get_min() const noexcept;
				value_type get_max() const noexcept;

				inline auto min()       noexcept { return utils::oop::proxy<spacing, false, value_type, &spacing::set_min, &spacing::get_min>{*this}; }
				inline auto min() const noexcept { return utils::oop::proxy<spacing, true , value_type, &spacing::set_min, &spacing::get_min>{*this}; }
				inline auto prf()       noexcept { return utils::oop::proxy<spacing, false, value_type, &spacing::set_prf, &spacing::get_prf>{*this}; }
				inline auto prf() const noexcept { return utils::oop::proxy<spacing, true , value_type, &spacing::set_prf, &spacing::get_prf>{*this}; }
				inline auto max()       noexcept { return utils::oop::proxy<spacing, false, value_type, &spacing::set_max, &spacing::get_max>{*this}; }
				inline auto max() const noexcept { return utils::oop::proxy<spacing, true , value_type, &spacing::set_max, &spacing::get_max>{*this}; }
			};
		}

	namespace details
		{
		float filter_nan(float a, float b) noexcept;

		float add_nan_aware(float a, float b) noexcept;
		float max_nan_aware(float a, float b) noexcept;
		float min_nan_aware(float a, float b) noexcept;

		//Note: std::clamp throws/UBs if min > max
		//Instead here we want to favour one
		float clamp_favour_min(float value, float min, float max) noexcept;

		//Note: std::clamp throws/UBs if min > max
		//Instead here we want to favour one
		float clamp_favour_min_nan_aware(float value, float min, float max) noexcept;

		float evaluate_element_position_for_alignment_based_container
			(
			const utils::math::rect<float>& container_rect,
			const utils::math::rect<float>& element_rect,
			const utils::alignment::horizontal& alignment
			) noexcept;

		float evaluate_element_position_for_alignment_based_container
			(
			const utils::math::rect<float>& container_rect,
			const utils::math::rect<float>& element_rect,
			const utils::alignment::vertical& alignment
			) noexcept;
		}
	}

#ifdef barnack_layout_implementation
#include "base_types.cpp"
#endif