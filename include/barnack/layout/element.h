#pragma once

#include <cmath>
#include <vector>
#include <functional>
#include <unordered_set>

#include "base_types.h"
#include "traversal.h"
#include "slot.h"

namespace barnack::layout
	{
	class element;
	using element_ref   = std  ::reference_wrapper<element    >;
	using element_own   = std  ::unique_ptr       <element    >;
	using element_obs   = utils::observer_ptr     <element    >;
	using elements_refs = std  ::vector           <element_ref>;

	class element
		{
		public:
			friend class traversal;

			template <std::derived_from<slot> slot_T>
			friend class layout::details::slot_access_helper;

			custom::sizes custom_sizes;

		private:
			utils::math::rect<float> rect_content{0.f, 0.f, 0.f, 0.f};
			element_obs parent_obs;

		public:
			const utils::math::rect<float>& get_rect_content() const noexcept;
			element_obs get_parent() const noexcept;

			virtual bool visit(const std::function<bool(element& element)>& callback) noexcept;

			void resize_checked(const utils::math::vec2f& size);

			virtual void resize    (const utils::math::vec2f& max_size);
			virtual void reposition(const utils::math::vec2f& position) noexcept;

			//Get sizes that depend on the evaluated content AND custom sizes.
			utils::math::vec2f get_size_min() const noexcept;
			//Get sizes that depend on the evaluated content AND custom sizes.
			utils::math::vec2f get_size_max() const noexcept;
			//Get sizes that depend on the evaluated content AND custom sizes.
			utils::math::vec2f get_size_prf() const noexcept;

			//Clear evaluated content sizes and invalidates cache in each parent up to the root.
			void clear_cached_min() const noexcept;
			//Clear evaluated content sizes and invalidates cache in each parent up to the root.
			void clear_cached_max() const noexcept;
			//Clear evaluated content sizes and invalidates cache in each parent up to the root.
			void clear_cached_prf() const noexcept;

			//Replace evaluated content sizes and invalidates cache in each parent up to the root.
			void update_cached_min(const utils::math::vec2f& new_values) const noexcept;
			//Replace evaluated content sizes and invalidates cache in each parent up to the root.
			void update_cached_max(const utils::math::vec2f& new_values) const noexcept;
			//Replace evaluated content sizes and invalidates cache in each parent up to the root.
			void update_cached_prf(const utils::math::vec2f& new_values) const noexcept;

			inline constexpr operator bool() const noexcept { return true; }

		protected:
			//Evaluate sizes depending on the content. Does NOT need to take custom sizes into account.
			virtual utils::math::vec2f evaluate_size_min() const noexcept;
			//Evaluate sizes depending on the content. Does NOT need to take custom sizes into account.
			virtual utils::math::vec2f evaluate_size_max() const noexcept;
			//Evaluate sizes depending on the content. Does NOT need to take custom sizes into account.
			virtual utils::math::vec2f evaluate_size_prf() const noexcept;

		private:
			//Private rather than protected so child classes are forced to call the clear cache function.
			//Writing here directly would not invalidate the size cache of parent nodes in the layout tree.
			mutable std::optional<utils::math::vec2f> cached_min_opt;
			mutable std::optional<utils::math::vec2f> cached_max_opt;
			mutable std::optional<utils::math::vec2f> cached_prf_opt;

			utils::math::vec2f get_cached_or_evaluate_size_min() const noexcept;
			utils::math::vec2f get_cached_or_evaluate_size_max() const noexcept;
			utils::math::vec2f get_cached_or_evaluate_size_prf() const noexcept;

		public:
			virtual void evaluate_traversal_for_children(traversal& traversal, traversal::node& previously_visited_my_node, const utils::math::vec2f& position) noexcept;
			virtual void evaluate_traversal_for_children(traversal::node& my_node, const utils::math::vec2f& position) noexcept;
		
			// Returns false if not visited and must be added to the to left roots tree
			bool evaluate_traversal(traversal& traversal, traversal::node& previously_visited_parent_node, const utils::math::vec2f& position) noexcept;
			void evaluate_traversal(traversal::node& newly_visited_parent_node, const utils::math::vec2f& position) noexcept;
		};

	struct owner : element {};
	}

#ifdef barnack_implementation
#include "element.cpp"
#endif