#pragma once

#include <cmath>
#include <vector>
#include <functional>
#include <unordered_set>

#include "base_types.h"

namespace barnack::layout
	{
	class element;
	using element_ref   = std  ::reference_wrapper<element    >;
	using element_own   = std  ::unique_ptr       <element    >;
	using element_obs   = utils::observer_ptr     <element    >;
	using elements_refs = std  ::vector           <element_ref>;

	class traversal
		{
		public:
			struct node
				{
				element_obs element_obs;
				std::vector<node> children;

				operator bool() const noexcept;
				bool has_value() const noexcept;
				
				/// <summary>
				/// 
				/// </summary>
				/// <param name="callback">bool(layout::element& element), return false if the visit shouldn't continue in depth.</param>
				void visit(this auto&& self, auto callback) noexcept
					{
					if (self.has_value())
						{
						if (callback(*self.element_obs))
							{
							for (auto& child : self.children)
								{
								child.visit(callback);
								}
							}
						}
					}

				/// <summary>
				/// 
				/// </summary>
				/// <param name="callback">bool(layout::element& element, size_t depth), return false if the visit shouldn't continue in depth.</param>
				/// <param name="depth"></param>
				void visit_with_depth(this auto&& self, auto callback, size_t depth = 0) noexcept
					{
					if (self.has_value())
						{
						if (callback(*self.element_obs, depth))
							{
							for (auto& child : self.children)
								{
								child.visit_with_depth(callback, depth + 1);
								}
							}
						}
					}
				};

		public:
			node _previously_visited;
			std::vector<node> _newly_visited_roots;
			std::vector<utils::observer_ptr<node>> _newly_visited_roots_parents;
			std::vector<node> _left_roots;

		public:
			const node& previously_visited() noexcept;

			const std::vector<node>& newly_visited() noexcept;

			const std::vector<node>& left() noexcept;

		private:
			void prepare_begin() noexcept;

		public:
			bool begin(element& root, const utils::math::vec2f& position) noexcept;
		};
	}



#ifdef barnack_implementation
#include "traversal.cpp"
#endif