#pragma once

#include <cmath>

#include <utils/math/vec.h>
#include <utils/math/rect.h>
#include <utils/math/constants.h>
#include <utils/oop/proxy.h>

namespace barnack::layout
	{
	namespace constraints
		{
		class monodirectional
			{
			public:
				struct unchecked
					{
					float min{utils::math::constants::fnan}; //can be [   0, _max] or fnan
					float prf{utils::math::constants::fnan}; //can be [_min, finf] or fnan
					float max{utils::math::constants::fnan}; //can be [_min, _max] or fnan
					bool validate() const noexcept;
					bool validate_min_max() const noexcept;
					bool validate_min_prf() const noexcept;
					bool validate_max_prf() const noexcept;
					static bool validate(const float min, const float prf, float max) noexcept;
					static bool validate_min_max(float min, float max) noexcept;
					static bool validate_min_prf(float min, float prf) noexcept;
					static bool validate_max_prf(float max, float prf) noexcept;
					};

				monodirectional() = default;
				monodirectional(const float _min, const float _max);
				monodirectional(const float _min, const float _prf, float _max);

				/// <summary> Must be [0.f, max()]</summary>
				void set_min(const float val);
				/// <summary> Must be [min(), max()]</summary>
				void set_prf(const float val);
				/// <summary> Must be [min(), finf]</summary>
				void set_max(const float& val);

				[[nodiscard]] float get_min() const noexcept;
				[[nodiscard]] float get_prf() const noexcept;
				[[nodiscard]] float get_max() const noexcept;

				inline auto min()       noexcept { return utils::oop::proxy<monodirectional, false, float, &monodirectional::set_min, &monodirectional::get_min>{*this}; }
				inline auto min() const noexcept { return utils::oop::proxy<monodirectional, true , float, &monodirectional::set_min, &monodirectional::get_min>{*this}; }
				inline auto prf()       noexcept { return utils::oop::proxy<monodirectional, false, float, &monodirectional::set_prf, &monodirectional::get_prf>{*this}; }
				inline auto prf() const noexcept { return utils::oop::proxy<monodirectional, true , float, &monodirectional::set_prf, &monodirectional::get_prf>{*this}; }
				inline auto max()       noexcept { return utils::oop::proxy<monodirectional, false, float, &monodirectional::set_max, &monodirectional::get_max>{*this}; }
				inline auto max() const noexcept { return utils::oop::proxy<monodirectional, true , float, &monodirectional::set_max, &monodirectional::get_max>{*this}; }

			private:
				unchecked values;
			};

		class bidirectional
			{
			public:
				using value_type = utils::math::vec2f;
				monodirectional x;
				monodirectional y;

				bidirectional() = default;
				bidirectional(const monodirectional& x, const monodirectional& y);
				bidirectional(const value_type& min, const value_type& max);
				bidirectional(const value_type& min, const value_type& prf, const value_type& max);

				void set_min(const value_type& val);
				void set_prf(const value_type& val);
				void set_max(const value_type& val);

				[[nodiscard]] value_type get_prf() const noexcept;
				[[nodiscard]] value_type get_min() const noexcept;
				[[nodiscard]] value_type get_max() const noexcept;

				inline auto min()       noexcept { return utils::oop::proxy<bidirectional, false, value_type, &bidirectional::set_min, &bidirectional::get_min>{*this}; }
				inline auto min() const noexcept { return utils::oop::proxy<bidirectional, true , value_type, &bidirectional::set_min, &bidirectional::get_min>{*this}; }
				inline auto prf()       noexcept { return utils::oop::proxy<bidirectional, false, value_type, &bidirectional::set_prf, &bidirectional::get_prf>{*this}; }
				inline auto prf() const noexcept { return utils::oop::proxy<bidirectional, true , value_type, &bidirectional::set_prf, &bidirectional::get_prf>{*this}; }
				inline auto max()       noexcept { return utils::oop::proxy<bidirectional, false, value_type, &bidirectional::set_max, &bidirectional::get_max>{*this}; }
				inline auto max() const noexcept { return utils::oop::proxy<bidirectional, true , value_type, &bidirectional::set_max, &bidirectional::get_max>{*this}; }
			};
	
		class spacing
			{
			public:
				using value_type = utils::math::rect_spacing<float>;

				monodirectional ll;
				monodirectional up;
				monodirectional rr;
				monodirectional dw;

				spacing() = default;
				spacing(const monodirectional& ll, const monodirectional& up, const monodirectional& rr, const monodirectional& dw);
				spacing(const value_type& min, const value_type& max);
				spacing(const value_type& min, const value_type& prf, const value_type& max);

				void set_min(const value_type& val);
				void set_prf(const value_type& val);
				void set_max(const value_type& val);

				[[nodiscard]] value_type get_prf() const noexcept;
				[[nodiscard]] value_type get_min() const noexcept;
				[[nodiscard]] value_type get_max() const noexcept;

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
		float filter_nan   (float a, float b) noexcept;
		float add_nan_aware(float a, float b) noexcept;
		float max_nan_aware(float a, float b) noexcept;
		float min_nan_aware(float a, float b) noexcept;
		float avg_nan_aware(float a, float b) noexcept;

		utils::math::vec2f filter_nan   (const utils::math::vec2f& a, const utils::math::vec2f& b) noexcept;
		utils::math::vec2f add_nan_aware(const utils::math::vec2f& a, const utils::math::vec2f& b) noexcept;
		utils::math::vec2f max_nan_aware(const utils::math::vec2f& a, const utils::math::vec2f& b) noexcept;
		utils::math::vec2f min_nan_aware(const utils::math::vec2f& a, const utils::math::vec2f& b) noexcept;
		utils::math::vec2f avg_nan_aware(const utils::math::vec2f& a, const utils::math::vec2f& b) noexcept;


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

#ifdef barnack_implementation
#include "base_types.cpp"
#endif