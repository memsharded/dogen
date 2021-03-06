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
#include "dogen/quilt.cpp/types/formatters/hash/variant_helper_stitch.hpp"

namespace dogen {
namespace quilt {
namespace cpp {
namespace formatters {
namespace hash {

void variant_helper_stitch(
    nested_type_formatting_assistant& a,
    const formattables::nested_type_info& t) {
a.stream() << std::endl;
a.stream() << "struct " << t.complete_identifiable_name() << "_visitor : public boost::static_visitor<> {" << std::endl;
a.stream() << "    " << t.complete_identifiable_name() << "_visitor() : hash(0) {}" << std::endl;
    const auto children(t.children());
    for (const auto& c : children) {
a.stream() << "    void operator()(const " << c.name() << (c.is_primitive() ? "" : "&") << " v) const {" << std::endl;
        if (!a.requires_hashing_helper_method(c))
a.stream() << "        combine(hash, v);" << std::endl;
        else
a.stream() << "        combine(hash, hash_" << c.complete_identifiable_name() << "(v));" << std::endl;
a.stream() << "    }" << std::endl;
a.stream() << std::endl;
    }
a.stream() << "    mutable std::size_t hash;" << std::endl;
a.stream() << "};" << std::endl;
a.stream() << std::endl;
a.stream() << "inline std::size_t hash_" << t.complete_identifiable_name() << "(const " << t.complete_name() << "& v) {" << std::endl;
a.stream() << "    " << t.complete_identifiable_name() << "_visitor vis;" << std::endl;
a.stream() << "    boost::apply_visitor(vis, v);" << std::endl;
a.stream() << "    return vis.hash;" << std::endl;
a.stream() << "}" << std::endl;
}
} } } } }
