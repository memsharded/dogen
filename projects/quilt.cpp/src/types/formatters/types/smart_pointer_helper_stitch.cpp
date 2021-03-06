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
#include "dogen/quilt.cpp/types/formatters/types/traits.hpp"
#include "dogen/quilt.cpp/types/formatters/assistant.hpp"
#include "dogen/quilt.cpp/types/formatters/types/smart_pointer_helper_stitch.hpp"

namespace dogen {
namespace quilt {
namespace cpp {
namespace formatters {
namespace types {

std::string smart_pointer_helper::family() const {
    static std::string r("Dereferenceable");
    return r;
}

std::string smart_pointer_helper::owning_formatter() const {
    return traits::class_implementation_formatter_name();
}

bool smart_pointer_helper::requires_explicit_call() const {
    return false;
}

std::string smart_pointer_helper::function_name() const {
    static std::string r("operator==");
    return r;
}

std::string smart_pointer_helper::helper_name() const {
    static std::string r(owning_formatter() + "." + family());
    return r;
}

bool smart_pointer_helper::is_enabled(const assistant& /*a*/,
    const formattables::class_info& /*owner*/) const {
    return true;
}

void smart_pointer_helper::
format(assistant& a, const yarn::name_tree& nt) const {
    {
        const auto ns(a.make_namespaces(nt.parent()));
        auto snf(a.make_scoped_namespace_formatter(ns));
a.stream() << std::endl;
a.stream() << "inline bool operator==(const " << a.make_qualified_name(nt.parent()) << "& lhs," << std::endl;
a.stream() << "const " << a.make_qualified_name(nt.parent()) << "& rhs) {" << std::endl;
a.stream() << "    return (!lhs && !rhs) ||(lhs && rhs && (*lhs == *rhs));" << std::endl;
a.stream() << "}" << std::endl;
a.stream() << std::endl;
    }
}

void smart_pointer_helper_stitch(
    nested_type_formatting_assistant& a,
    const formattables::nested_type_info& t) {

    {
        auto snf(a.make_scoped_namespace_formatter(t.namespaces()));
        const auto containee(t.children().front());
a.stream() << std::endl;
a.stream() << "inline bool operator==(const " << t.complete_name() << "& lhs," << std::endl;
a.stream() << "const " << t.complete_name() << "& rhs) {" << std::endl;
a.stream() << "    return (!lhs && !rhs) ||(lhs && rhs && (*lhs == *rhs));" << std::endl;
a.stream() << "}" << std::endl;
a.stream() << std::endl;
    }
a.stream() << std::endl;
}
} } } } }
