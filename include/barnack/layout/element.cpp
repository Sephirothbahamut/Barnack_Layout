#include "element.h"

namespace barnack::layout
	{
	class element;
	using element_ref   = std  ::reference_wrapper<element    >;
	using element_own   = std  ::unique_ptr       <element    >;
	using element_obs   = utils::observer_ptr     <element    >;
	using elements_refs = std  ::vector           <element_ref>;

	class element;

	const utils::math::rect<float>& element::get_rect_content() const noexcept { return rect_content; }
	element_obs element::get_parent() const noexcept { return parent_obs; }

	bool element::visit(const std::function<bool(element& element)>& callback) noexcept
		{
		return callback(*this);
		}

	void element::resize_checked(const utils::math::vec2f& size)
		{
		const utils::math::vec2f min_a{get_size_min()};
		const utils::math::vec2f max_a{get_size_max()};

		if (size.x() < min_a.x() /*|| size.x() > max.x()*/ ||
			size.y() < min_a.y() /*|| size.y() > max.y()*/)
			{
			//throw std::runtime_error{"Invalid resizing values."};
			}
		const utils::math::vec2f new_size
			{
			std::max(std::min(size.x(), max_a.x()), min_a.x()),
			std::max(std::min(size.y(), max_a.y()), min_a.y())
			};
		rect_content.proxy_size<utils::alignment::create::top_left()>() = new_size;
		}

	void element::resize    (const utils::math::vec2f& max_size)
		{
		rect_content.proxy_size<utils::alignment::create::top_left()>() = max_size;
		}

	void element::reposition(const utils::math::vec2f& position) noexcept
		{
		rect_content.proxy_position() = position;
		}

	utils::math::vec2f element::get_size_min() const noexcept
		{
		if (!cached_min_opt.has_value()) 
			{
			cached_min_opt = evaluate_size_min();
			} 
		return cached_min_opt.value(); 
		}

	utils::math::vec2f element::get_size_prf() const noexcept
		{
		if (!cached_prf_opt.has_value()) 
			{
			cached_prf_opt = evaluate_size_prf();
			} 
		return cached_prf_opt.value(); 
		}

	utils::math::vec2f element::get_size_max() const noexcept
		{
		if (!cached_max_opt.has_value()) 
			{
			cached_max_opt = evaluate_size_max();
			} 
		return cached_max_opt.value(); 
		}

	void element::clear_cached_min() const noexcept { cached_min_opt = std::nullopt; if (parent_obs) { parent_obs->clear_cached_min(); } }
	void element::clear_cached_prf() const noexcept { cached_prf_opt = std::nullopt; if (parent_obs) { parent_obs->clear_cached_prf(); } }
	void element::clear_cached_max() const noexcept { cached_max_opt = std::nullopt; if (parent_obs) { parent_obs->clear_cached_max(); } }
	
	void element::update_cached_min(const utils::math::vec2f& new_values) const noexcept { cached_min_opt = new_values; if (parent_obs) { parent_obs->clear_cached_min(); } }
	void element::update_cached_prf(const utils::math::vec2f& new_values) const noexcept { cached_min_opt = new_values; if (parent_obs) { parent_obs->clear_cached_prf(); } }
	void element::update_cached_max(const utils::math::vec2f& new_values) const noexcept { cached_min_opt = new_values; if (parent_obs) { parent_obs->clear_cached_max(); } }

	utils::math::vec2f element::evaluate_size_min() const noexcept { return {                         0.f,                          0.f}; }
	utils::math::vec2f element::evaluate_size_prf() const noexcept { return {                        32.f,                         32.f}; }
	utils::math::vec2f element::evaluate_size_max() const noexcept { return {utils::math::constants::finf, utils::math::constants::finf}; }

	void element::evaluate_traversal_for_children(traversal& traversal, traversal::node& previously_visited_my_node, const utils::math::vec2f& position) noexcept
		{
		}
	void element::evaluate_traversal_for_children(traversal::node& my_node, const utils::math::vec2f& position) noexcept
		{
		}
		
	bool element::evaluate_traversal(traversal& traversal, traversal::node& previously_visited_parent_node, const utils::math::vec2f& position) noexcept
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

	void element::evaluate_traversal(traversal::node& newly_visited_parent_node, const utils::math::vec2f& position) noexcept
		{
		if (rect_content.contains(position))
			{
			auto& my_node{newly_visited_parent_node.children.emplace_back(traversal::node{.element_obs{this}})};
			evaluate_traversal_for_children(my_node, position);
			}
		}
	}

