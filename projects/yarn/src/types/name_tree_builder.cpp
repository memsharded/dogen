/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2012-2015 Marco Craveiro <marco.craveiro@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */
#include <sstream>
#include <boost/make_shared.hpp>
#include "dogen/utility/log/logger.hpp"
#include "dogen/utility/io/unordered_set_io.hpp"
#include "dogen/utility/io/list_io.hpp"
#include "dogen/yarn/io/name_io.hpp"
#include "dogen/yarn/io/location_io.hpp"
#include "dogen/yarn/io/name_tree_io.hpp"
#include "dogen/yarn/types/name_builder.hpp"
#include "dogen/yarn/types/name_pretty_printer.hpp"
#include "dogen/yarn/types/name_tree_builder.hpp"

using namespace dogen::utility::log;

namespace {

auto lg(logger_factory("yarn.name_tree_builder"));
const char less_than('<');
const char greater_than('>');
const char space(' ');
const std::string comma_space(", ");

}

namespace dogen {
namespace yarn {

name_tree_builder::name_tree_builder(
    const std::unordered_set<std::string>& top_level_modules,
    const location& model_location)
    : top_level_modules_(top_level_modules),
      model_location_(model_location),
      root_(new node) {

    current_ = root_;
    BOOST_LOG_SEV(lg, debug) << "Initialised with settings:";
    BOOST_LOG_SEV(lg, debug) << " modules: " << top_level_modules_;
    BOOST_LOG_SEV(lg, debug) << " location: " << model_location_;
}

void name_tree_builder::add_name(const std::string& s) {
    BOOST_LOG_SEV(lg, debug) << "Pushing back name: " << s;
    names_.push_back(s);
}

void name_tree_builder::add_primitive(const std::string& s) {
    BOOST_LOG_SEV(lg, debug) << "Pushing back primitive :" << s;

    name_builder b;
    b.simple_name(s);
    current_->data(b.build());
}

void name_tree_builder::finish_current_node() {
    BOOST_LOG_SEV(lg, debug) << "Finishing current node. names: " << names_;

    /*
     * if there are no names, we do not have any work to do.
     */
    if (names_.empty())
        return;

    name_builder b;

    /*
     * If we have a single name, we are either referencing a type
     * defined in the global namespace (possibly in a different
     * model), or a type defined at the top-level in this model. As we
     * cannot tell the difference, we must fill in just the simple
     * name and let the resolver handle it properly later on.
     */
    const auto front(names_.front());
    if (names_.size() == 1) {
        b.simple_name(front);
        current_->data(b.build());
        names_.clear();
        BOOST_LOG_SEV(lg, debug) << "Simple name: " << front;
        return;
    }

    /*
     * Check to see if the first name matches a top-level module in
     * this model. If it does, we must be referencing a type in a
     * module in the current model. If it does not, we are referencing
     * a type on a different model, and this is the foreign model
     * name.
     */
    const auto i(top_level_modules_.find(front));
    if (i != top_level_modules_.end()) {
        b.model_name(model_location_);
        BOOST_LOG_SEV(lg, debug) << "Found module in current model: " << front;
    } else {
        BOOST_LOG_SEV(lg, debug) << "Foreign model name: " << front;
        b.model_name(front);
        names_.pop_front(); // consume the foreign model name.
    }

    /*
     * The back of the list must now be the type's simple name.
     */
    const auto back(names_.back());
    b.simple_name(back);
    BOOST_LOG_SEV(lg, debug) << "Simple name: " << back;
    names_.pop_back(); // consume the simple name

    if (!names_.empty()) {
        /*
         * Whatever is left, if anything, must be a path to non-top-level
         * modules defined within the model.
         */
        b.internal_modules(names_);
        BOOST_LOG_SEV(lg, debug) << "Internal modules: " << names_;
        names_.clear(); // consume internal modules
    }
    current_->data(b.build());
}

void name_tree_builder::start_children() {
    BOOST_LOG_SEV(lg, debug) << "Starting children.";

    /*
     * We are done building the parent's name so flush it.
     */
    finish_current_node();

    /*
     * Create a child node, link it up to its parent, link the parent
     * to the child and then make the child the current node.
     */
    auto child(boost::make_shared<node>());
    child->parent(current_);
    current_->children().push_back(child);
    current_ = child;
}

void name_tree_builder::next_child() {
    BOOST_LOG_SEV(lg, debug) << "Moving to next child.";

    /*
     * We are done building the current child's name so flush it.
     */
    finish_current_node();

    /*
     * We are currently sitting on a child node. We first need to move
     * back up to the parent; then add a new child node, link it to
     * the parent and link the parent to the child; finally, make the
     * new child the current node.
     */
    current_ = current_->parent();
    auto child(boost::make_shared<node>());
    child->parent(current_);
    current_->children().push_back(child);
    current_ = child;
}

void name_tree_builder::end_children() {
    BOOST_LOG_SEV(lg, debug) << "Children have ended.";

    /*
     * We are done building the current child's name so flush it.
     */
    finish_current_node();

    /*
     * All the children are done, so move back up to the parent.
     */
    current_ = current_->parent();
}

name_tree name_tree_builder::make_name_tree(const node& n) {
    BOOST_LOG_SEV(lg, debug) << "Node: " << n.data();

    name_tree r;
    r.parent(n.data());

    std::ostringstream s;
    name_pretty_printer p(printing_styles::scoped);
    s << p.print(n.data());

    bool is_first = true;
    for (const auto c : n.children()) {
        if (is_first)
            s << less_than;
        else
            s << comma_space;

        const auto cnt(make_name_tree(*c));
        s << cnt.unparsed_type();
        r.children().push_back(cnt);
        is_first = false;
    }

    if (!r.children().empty()) {
        /*
         * If the last child also had children, add a space between
         * template markers. Not really required for C++ 11 and above,
         * but we will leave it for now to avoid spurious differences.
         */
        const auto& ut(r.children().back().unparsed_type());
        if (ut[ut.length() - 1] == greater_than)
            s << space;
        s << greater_than;
    }

    r.unparsed_type(s.str());
    return r;
}

name_tree name_tree_builder::build() {
    BOOST_LOG_SEV(lg, debug) << "Started build";

    /*
     * Flush any pending work and build the name for it. This handles
     * the case of a parent without any children.
     */
    finish_current_node();

    /*
     * Convert the node representation into a tree representation.
     */
    name_tree r(make_name_tree(*root_));

    BOOST_LOG_SEV(lg, debug) << "Finished build. Final name: " << r;

    return r;
}

} }
