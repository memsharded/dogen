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
#include "dogen/formatters/types/sequence_formatter.hpp"
#include "dogen/quilt.cpp/types/formatters/io/inserter_implementation_helper_stitch.hpp"

namespace dogen {
namespace quilt {
namespace cpp {
namespace formatters {
namespace io {

void inserter_implementation_helper_stitch(
    assistant& a, const formattables::class_info& c,
    const bool inside_class) {

    if (c.requires_stream_manipulators()) {
a.stream() << "    boost::io::ios_flags_saver ifs(s);" << std::endl;
a.stream() << "    s.setf(std::ios_base::boolalpha);" << std::endl;
a.stream() << "    s.setf(std::ios::fixed, std::ios::floatfield);" << std::endl;
a.stream() << "    s.precision(6);" << std::endl;
a.stream() << "    s.setf(std::ios::showpoint);" << std::endl;
a.stream() << std::endl;
    }

    const bool no_parents_and_no_properties(c.parents().empty() &&
        c.all_properties().empty());
a.stream() << "    s << \" { \"" << std::endl;
a.stream() << "      << \"\\\"__type__\\\": \" << \"\\\"" << c.qualified_name() << "\\\"\"" << (no_parents_and_no_properties ? " << \" }\";" : " << \", \"") << std::endl;

    dogen::formatters::sequence_formatter sf(c.parents().size());
    sf.prefix_configuration().first("  << ").not_first("s << ");
    sf.element_separator("");
    for (const auto p : c.parents()) {
a.stream() << "    " << sf.prefix() << "\"\\\"__parent_" << sf.current_position() << "__\\\": \"" << sf.postfix() << ";" << std::endl;
a.stream() << "    " << p.name() << "::to_stream(s);" << std::endl;
        sf.next();
    }

    auto ntfa(a.make_nested_type_formatting_assistant());
    sf.reset(c.properties().size());

    if (!c.parents().empty())
        sf.prefix_configuration().first("s << \", \"\n      ");
    else
        sf.prefix_configuration().first("  ");
    sf.prefix_configuration().not_first("  ");
    sf.postfix_configuration().not_last(" << \", \"");
    sf.element_separator("");

    for (const auto p : c.properties()) {
        std::string variable_name;
        if (inside_class)
            variable_name = a.make_member_variable_name(p);
        else
            variable_name = "v." + a.make_getter_setter_name(p) + "()";

a.stream() << "    " << sf.prefix() << "<< \"\\\"" << p.name() << "\\\": \" << " << ntfa.streaming_for_type(p.type(), variable_name) << sf.postfix() << std::endl;
        sf.next();
    }

    if (!no_parents_and_no_properties) {
        if (!c.properties().empty())
a.stream() << "      << \" }\";" << std::endl;
        else
a.stream() << "    s << \" }\";" << std::endl;
    }

    if (!inside_class)
a.stream() << "    return(s);" << std::endl;
}
} } } } }
