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
#include "dogen/yarn/test_data/name_td.hpp"
#include "dogen/yarn.dia/test_data/context_td.hpp"
#include "dogen/yarn/test_data/intermediate_model_td.hpp"

namespace {

std::string create_std_string(const unsigned int position) {
    std::ostringstream s;
    s << "a_string_" << position;
    return s.str();
}

std::list<std::string> create_std_list_std_string(unsigned int position) {
    std::list<std::string> r;
    for (unsigned int i(0); i < 4; ++i) {
        r.push_back(create_std_string(position + i));
    }
    return r;
}

std::unordered_map<std::string, std::list<std::string> > create_std_unordered_map_std_string_std_list_std_string_(unsigned int position) {
    std::unordered_map<std::string, std::list<std::string> > r;
    for (unsigned int i(0); i < 4; ++i) {
        r.insert(std::make_pair(create_std_string(position + i), create_std_list_std_string(position + i)));
    }
    return r;
}

std::unordered_set<std::string> create_std_unordered_set_std_string(unsigned int position) {
    std::unordered_set<std::string> r;
    for (unsigned int i(0); i < 4; ++i) {
        r.insert(create_std_string(position + i));
    }
    return r;
}

dogen::yarn::name
create_dogen_yarn_name(const unsigned int position) {
    return dogen::yarn::name_generator::create(position);
}

std::unordered_map<std::string, dogen::yarn::name> create_std_unordered_map_std_string_dogen_yarn_name(unsigned int position) {
    std::unordered_map<std::string, dogen::yarn::name> r;
    for (unsigned int i(0); i < 4; ++i) {
        r.insert(std::make_pair(create_std_string(position + i), create_dogen_yarn_name(position + i)));
    }
    return r;
}

dogen::yarn::intermediate_model
create_dogen_yarn_intermediate_model(const unsigned int position) {
    return dogen::yarn::intermediate_model_generator::create(position);
}

}

namespace dogen {
namespace yarn {
namespace dia {

context_generator::context_generator() : position_(0) { }

void context_generator::
populate(const unsigned int position, result_type& v) {
    v.child_id_to_parent_ids(create_std_unordered_map_std_string_std_list_std_string_(position + 0));
    v.parent_ids(create_std_unordered_set_std_string(position + 1));
    v.id_to_name(create_std_unordered_map_std_string_dogen_yarn_name(position + 2));
    v.model(create_dogen_yarn_intermediate_model(position + 3));
}

context_generator::result_type
context_generator::create(const unsigned int position) {
    context r;
    context_generator::populate(position, r);
    return r;
}

context_generator::result_type*
context_generator::create_ptr(const unsigned int position) {
    context* p = new context();
    context_generator::populate(position, *p);
    return p;
}

context_generator::result_type
context_generator::operator()() {
    return create(position_++);
}

} } }
