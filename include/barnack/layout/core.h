#pragma once

#include <cmath>
#include <functional>

#include "base_types.h"

namespace barnack::layout::core
	{
	struct object_in_region_actions_t
		{
		/// <summary> Whether this UI element will be in the vector of items the mouse is over. </summary>
		bool self{false};
		/// <summary> Whether this UI element will prevent block the mouse-over visit tree from adding its children to the list. </summary>
		bool block{false};
		};

	struct object
		{
		virtual object_in_region_actions_t object_in_region_actions() noexcept { return {}; }
		};

	using objects_refs = std::vector<std::reference_wrapper<object>>;
	struct objects_in_region_t
		{
		core::objects_refs objects;
		bool should_continue;
		};

	class element
		{
		public:
			utils::observer_ptr<core::object> object_ptr;
			utils::math::rect<float> rect{0.f, 0.f, 0.f, 0.f};

			virtual core::objects_refs objects_in_region(const utils::math::vec2f& position) noexcept
				{
				const auto ret{object_in_region(position).objects};
				return ret;
				}

			void resize_checked(const utils::math::vec2f& size)
				{
				const utils::math::vec2f min_a{get_size_min()};
				const utils::math::vec2f max_a{get_size_max()};
				if (size.x() < min_a.x() /*|| size.x() > max.x()*/ ||
					size.y() < min_a.y() /*|| size.y() > max.y()*/)
					{
					throw std::runtime_error{"Invalid resizing values."};
					}
				const utils::math::vec2f new_size
					{
					std::min(size.x(), max_a.x()),
					std::min(size.y(), max_a.y())
					};
				rect.proxy_size<utils::alignment::create::top_left()>() = new_size;
				}

			virtual void resize    (const utils::math::vec2f& max_size) 
				{
				rect.proxy_size<utils::alignment::create::top_left()>() = max_size; 
				}

			virtual void reposition(const utils::math::vec2f& position) noexcept 
				{
				rect.proxy_position() = position; 
				}

			custom_sizes_t sizes;

			utils::math::vec2f get_size_min() const noexcept 
				{
				const utils::math::vec2f tmp{get_cached_or_evaluate_size_min()}; 
				const utils::math::vec2f ret{std::max(sizes.min_x(), tmp.x()), std::max(sizes.min_y(), tmp.y())};
				return ret;
				}
			utils::math::vec2f get_size_max() const noexcept 
				{ 
				const utils::math::vec2f tmp{get_cached_or_evaluate_size_max()}; 
				const utils::math::vec2f ret{std::min(sizes.max_x(), tmp.x()), std::min(sizes.max_y(), tmp.y())};
				return ret;
				}
			utils::math::vec2f get_size_prf() const noexcept 
				{ 
				utils::math::vec2f tmp{get_cached_or_evaluate_size_prf()};
				utils::math::vec2f min{get_size_min()};
				utils::math::vec2f max{get_size_max()};

				return
					{
					std::isnan(sizes.prf_x()) ? std::clamp(tmp.x(), min.x(), max.x()) : sizes.prf_x(), 
					std::isnan(sizes.prf_y()) ? std::clamp(tmp.y(), min.y(), max.y()) : sizes.prf_y()
					};
				}

			inline constexpr operator bool() const noexcept { return true; }

			virtual void traverse(const std::function<void(const element& element)>& callback) const noexcept
				{
				callback(*this);
				}


		protected:
			objects_in_region_t object_in_region(const utils::math::vec2f& position) noexcept
				{
				if (rect.contains(position))
					{
					if (object_ptr)
						{
						const auto actions{object_ptr->object_in_region_actions()};
						return {actions.self ? core::objects_refs{*object_ptr} : core::objects_refs{}, !actions.block};
						}
					}
				return {{}, true};
				}

			virtual utils::math::vec2f evaluate_size_min() const noexcept { return {  0.f,   0.f}; }
			virtual utils::math::vec2f evaluate_size_max() const noexcept { return {utils::math::constants::finf, utils::math::constants::finf}; }
			virtual utils::math::vec2f evaluate_size_prf() const noexcept
				{
				utils::math::vec2f min{get_size_min()};
				utils::math::vec2f max{get_size_max()};
				return
					{
					max.x() == utils::math::constants::finf ? std::max(128.f, min.x()) : min.x() + ((max.x() - min.x()) / 2.f),
					max.y() == utils::math::constants::finf ? std::max( 64.f, min.y()) : min.y() + ((max.y() - min.y()) / 2.f)
					};
				}

		private:
			mutable std::optional<utils::math::vec2f> cached_min_opt;
			mutable std::optional<utils::math::vec2f> cached_max_opt;
			mutable std::optional<utils::math::vec2f> cached_prf_opt;

			utils::math::vec2f get_cached_or_evaluate_size_min() const noexcept 
				{
				if (!cached_min_opt.has_value()) 
					{
					cached_min_opt = evaluate_size_min();
					} 
				return cached_min_opt.value(); 
				}

			utils::math::vec2f get_cached_or_evaluate_size_max() const noexcept 
				{
				if (!cached_max_opt.has_value()) 
					{
					cached_max_opt = evaluate_size_max();
					} 
				return cached_max_opt.value(); 
				}

			utils::math::vec2f get_cached_or_evaluate_size_prf() const noexcept 
				{
				if (!cached_prf_opt.has_value()) 
					{
					cached_prf_opt = evaluate_size_min();
					} 
				return cached_prf_opt.value(); 
				}
		};


	
	class owner : public element
		{
		public:
			virtual void push(std::unique_ptr<element>&& element) = 0;
		};

	class wrapper : public owner
		{
		public:
			virtual core::objects_refs objects_in_region(const utils::math::vec2f& position) noexcept override
				{
				auto object_in_region{element::object_in_region(position)};
				if (object_in_region.should_continue)
					{
					const auto tmp{element->objects_in_region(position)};
					object_in_region.objects.insert(object_in_region.objects.end(), tmp.begin(), tmp.end());
					}
				return object_in_region.objects;
				}

			template <typename T, typename ...Args>
			T& emplace(Args... args) noexcept 
				{ 
				std  ::  unique_ptr<T> tmp{std::make_unique<T>(std::forward<Args>(args)...)};
				utils::observer_ptr<T> ptr{tmp.get()};
				element = std::move(tmp);
				return *ptr;
				}

			virtual void push(std::unique_ptr<element>&& element) noexcept final override { this->element = std::move(element); }

			virtual void traverse(const std::function<void(const element& element)>& callback) const noexcept
				{
				callback(*this);
				if (element)
					{
					element->traverse(callback);
					}
				}

		protected:
			std::unique_ptr<element> element;
		};

	class container : public owner
		{
		public:
			virtual core::objects_refs objects_in_region(const utils::math::vec2f& position) noexcept override
				{
				auto object_in_region{element::object_in_region(position)};
				if (object_in_region.should_continue)
					{
					for (auto& element_ptr : elements)
						{
						auto& element = *element_ptr;
						const auto tmp{element.objects_in_region(position)};
						object_in_region.objects.insert(object_in_region.objects.end(), tmp.begin(), tmp.end());
						}
					}
				return object_in_region.objects;
				}

			template <typename T, typename ...Args>
			T& emplace(Args... args) noexcept
				{
				std::unique_ptr<T> tmp{std::make_unique<T>(std::forward<Args>(args)...)};
				utils::observer_ptr<T> ptr{tmp.get()};
				elements.push_back(std::move(tmp));
				return *ptr;
				}

			virtual void push(std::unique_ptr<element>&& element) noexcept final override { this->elements.push_back(std::move(element)); }

			virtual void traverse(const std::function<void(const element& element)>& callback) const noexcept
				{
				callback(*this);
				for (const auto& element : elements)
					{
					if (element) 
						{
						element->traverse(callback);
						}
					}
				}

		protected:
			std::vector<std::unique_ptr<element>> elements;
		};
	}

