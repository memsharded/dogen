/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2012 Kitanda <info@kitanda.co.uk>
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
#include "dogen/dynamic/expansion/types/default_value_expander.hpp"
#include "dogen/dynamic/expansion/types/root_object_copier.hpp"

namespace dogen {
namespace dynamic {
namespace expansion {

root_object_copier::~root_object_copier() noexcept { }

std::string root_object_copier::name() const {
    static std::string name("root_object_copier");
    return name;
}

const std::forward_list<std::string>&
root_object_copier::dependencies() const {
    static std::forward_list<std::string> r { "default_value_expander" };
    return r;
}

void root_object_copier::
expand(const expansion_context& /*ec*/, schema::object& /*o*/) const {
}

} } }