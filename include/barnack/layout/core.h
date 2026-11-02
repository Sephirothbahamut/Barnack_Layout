#pragma once

#include <cmath>
#include <vector>
#include <functional>
#include <unordered_set>

#include "base_types.h"




namespace barnack::layout::core
	{
	class element;
	using element_ref   = std  ::reference_wrapper<element    >;
	using element_own   = std  ::unique_ptr       <element    >;
	using element_obs   = utils::observer_ptr     <element    >;
	using elements_refs = std  ::vector           <element_ref>;

	class element;


	class traversal
		{
		public:
			struct node
				{
				element_obs element_obs;
				std::vector<node> children;

				operator bool() const noexcept { return has_value(); }
				bool has_value() const noexcept { return element_obs != nullptr; }

				/// <summary>
				/// 
				/// </summary>
				/// <param name="callback">bool(core::element& element), return false if the visit shouldn't continue in depth.</param>
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
				/// <param name="callback">bool(core::element& element, size_t depth), return false if the visit shouldn't continue in depth.</param>
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
			const node& previously_visited() noexcept
				{
				return _previously_visited;
				}

			const auto& newly_visited() noexcept
				{
				return _newly_visited_roots;
				}

			const auto& left() noexcept
				{
				return _left_roots;
				}

		private:
			void prepare_begin() noexcept
				{
				if (!_previously_visited.has_value())
					{
					if(!_newly_visited_roots.empty())
						{
						_previously_visited = std::move(_newly_visited_roots[0]);
						}
					}
				else
					{
					assert(_newly_visited_roots.size() == _newly_visited_roots_parents.size());
					for (size_t i{0}; i < _newly_visited_roots.size(); i++)
						{
						auto& parent{*_newly_visited_roots_parents[i]};
						parent.children.emplace_back(std::move(_newly_visited_roots[i]));
						}
					}

				_newly_visited_roots        .clear();
				_newly_visited_roots_parents.clear();
				_left_roots                 .clear();
				}

		public:
			bool begin(element& root, const utils::math::vec2f& position) noexcept;
		};

	class element
		{
		public:
			friend class traversal;
			custom::sizes custom_sizes;
			utils::math::rect<float> rect_content{0.f, 0.f, 0.f, 0.f};

			virtual void visit(const std::function<void(const element& element)>& callback) noexcept
				{
				callback(*this);
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
				rect_content.proxy_size<utils::alignment::create::top_left()>() = new_size;
				}

			virtual void resize    (const utils::math::vec2f& max_size) 
				{
				rect_content.proxy_size<utils::alignment::create::top_left()>() = max_size;
				}

			virtual void reposition(const utils::math::vec2f& position) noexcept 
				{
				rect_content.proxy_position() = position;
				}

			utils::math::vec2f get_size_min() const noexcept 
				{
				const utils::math::vec2f tmp{get_cached_or_evaluate_size_min()}; 
				const utils::math::vec2f ret{std::min(std::max(custom_sizes.x.get_min(), tmp.x()), custom_sizes.x.get_max()), std::min(std::max(custom_sizes.y.get_min(), tmp.y()), custom_sizes.x.get_max())};
				return ret;
				}
			utils::math::vec2f get_size_max() const noexcept 
				{ 
				const utils::math::vec2f tmp{get_cached_or_evaluate_size_max()}; 
				const utils::math::vec2f ret{std::max(std::min(custom_sizes.x.get_max(), tmp.x()), custom_sizes.x.get_min()), std::max(std::min(custom_sizes.y.get_max(), tmp.y()), custom_sizes.x.get_min())};
				return ret;
				}
			utils::math::vec2f get_size_prf() const noexcept 
				{ 
				const utils::math::vec2f tmp{get_cached_or_evaluate_size_prf()};
				const utils::math::vec2f min{get_size_min()};
				const utils::math::vec2f max{get_size_max()};
				
				const auto custom_sizes_prf{custom_sizes.get_prf()};

				const utils::math::vec2f ret
					{
					[&]() -> float
						{
						if (!std::isnan(custom_sizes_prf.x())) { return custom_sizes_prf.x(); }
						else
							{
							if (!std::isnan(tmp.x()))
								{
								return layout::details::clamp_favour_min(tmp.x(), min.x(), max.x());
								}
							return utils::math::constants::fnan;
							}
						}(),
					[&]() -> float
						{
						if (!std::isnan(custom_sizes_prf.y())) { return custom_sizes_prf.x(); }
						else
							{
							if (!std::isnan(tmp.y()))
								{
								return layout::details::clamp_favour_min(tmp.y(), min.y(), max.y());
								}
							return utils::math::constants::fnan;
							}
						}()
					};

				return ret;
				}

			inline constexpr operator bool() const noexcept { return true; }

		protected:

			virtual utils::math::vec2f evaluate_size_min() const noexcept { return {  0.f,   0.f}; }
			virtual utils::math::vec2f evaluate_size_max() const noexcept { return {utils::math::constants::finf, utils::math::constants::finf}; }
			virtual utils::math::vec2f evaluate_size_prf() const noexcept { return {utils::math::constants::fnan, utils::math::constants::fnan}; }

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
					cached_prf_opt = evaluate_size_prf();
					} 
				return cached_prf_opt.value(); 
				}

		public:
			virtual void evaluate_traversal_for_children(traversal& traversal, traversal::node& previously_visited_my_node, const utils::math::vec2f& position) noexcept
				{
				}
			virtual void evaluate_traversal_for_children(traversal::node& my_node, const utils::math::vec2f& position) noexcept
				{
				}
		
			// Returns false if not visited and must be added to the to left roots tree
			bool evaluate_traversal(traversal& traversal, traversal::node& previously_visited_parent_node, const utils::math::vec2f& position) noexcept
				{
				if (rect_content.contains(position))
					{
					const auto it{std::find_if(previously_visited_parent_node.children.begin(), previously_visited_parent_node.children.end(), [this](const traversal::node& node) { return node.element_obs == this; })};
					if (it != previously_visited_parent_node.children.end()) //I was previously visited
						{
						auto& my_node{*it};
						evaluate_traversal_for_children(traversal, my_node, position);
						}
					else //I'm newly visited
						{
						auto& my_node{traversal._newly_visited_roots.emplace_back(traversal::node{.element_obs{this}})};
						traversal._newly_visited_roots_parents.push_back(std::addressof(previously_visited_parent_node));
						evaluate_traversal_for_children(my_node, position);
						}
					return true;
					}
				return false;
				}
			void evaluate_traversal(traversal::node& newly_visited_parent_node, const utils::math::vec2f& position) noexcept
				{
				if (rect_content.contains(position))
					{
					auto& my_node{newly_visited_parent_node.children.emplace_back(traversal::node{.element_obs{this}})};
					evaluate_traversal_for_children(my_node, position);
					}
				}
		};


	bool traversal::begin(element& root, const utils::math::vec2f& position) noexcept
		{
		prepare_begin();
	
		const bool needs_new_root{[&]()
			{
			if (!_previously_visited.has_value())
				{
				return true;
				}
			else if (_previously_visited.element_obs != std::addressof(root))
				{
				_left_roots.emplace_back(std::move(_previously_visited));
				_previously_visited.element_obs = nullptr;
				return true;
				}
			return false;
			}()};
		
		if (needs_new_root)
			{
			if (root.rect_content.contains(position))
				{
				auto& newly_visited_root{_newly_visited_roots.emplace_back(node{.element_obs{std::addressof(root)}})};
				root.evaluate_traversal_for_children(newly_visited_root, position);
				}
			}
		else //_previously_visited is already this root
			{
			root.evaluate_traversal_for_children(*this, _previously_visited, position);
			}

		return !_left_roots.empty() || !_newly_visited_roots.empty();
		}

	struct slot
		{
		core::element_own child_own;

		template <typename T, typename ...Args>
		T& emplace(Args&&... args) noexcept
			{
			std::unique_ptr<T> tmp{std::make_unique<T>(std::forward<Args>(args)...)};
			utils::observer_ptr<T> ptr{tmp.get()};
			child_own = std::move(tmp);
			return *ptr;
			}

		template <typename T>
		utils::observer_ptr<T> move_in(std::unique_ptr<element>&& new_child_own) noexcept
			{
			assert(new_child_own);
			utils::observer_ptr<T> ptr{new_child_own.get()};
			child_own = std::move(new_child_own);
			return ptr;
			}
		};

	template <typename alignment_T>
	struct slot_with_alignment : slot
		{
		using alignment_t = alignment_T;
		alignment_t alignment{utils::alignment::default_value<alignment_t>()};

		slot_with_alignment(alignment_t alignment) : alignment{alignment} {}
		};

	namespace details
		{
		/// <summary> Used to guarantee that a child exists.</summary>
		/// <typeparam name="slot_T"></typeparam>
		template <std::derived_from<slot> slot_T>
		class emplacement_helper : public utils::oop::non_copyable, public utils::oop::non_movable
			{
			public:
				using slot_t = slot_T;

			private:
				slot_t& slot;

			public:
				emplacement_helper(slot_t& slot) : slot{slot} {}
				~emplacement_helper() { assert(this->slot.child_own); }

				template <typename T, typename ...Args>
				T& emplace(Args&&... args) noexcept
					{
					return slot.emplace<T>(std::forward<Args>(args)...);
					}

				template <typename T>
				utils::observer_ptr<T> move_in(std::unique_ptr<element>&& new_child_own) noexcept
					{
					return slot.move_in<T>(std::move(new_child_own));
					}
			};
		}

	struct owner : element {};
	}

