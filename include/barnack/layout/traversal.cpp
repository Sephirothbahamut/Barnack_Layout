#include "traversal.h"

#include "element.h"

namespace barnack::layout
	{
	traversal::node::operator bool() const noexcept { return has_value(); }
	bool traversal::node::has_value() const noexcept { return element_obs != nullptr; }

	const traversal::node& traversal::previously_visited() noexcept
		{
		return _previously_visited;
		}

	const std::vector<traversal::node>& traversal::newly_visited() noexcept
		{
		return _newly_visited_roots;
		}

	const std::vector<traversal::node>& traversal::left() noexcept
		{
		return _left_roots;
		}

	void traversal::prepare_begin() noexcept
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
			if (root.hit_test(position))
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
	}

