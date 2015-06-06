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
#include <sstream>
#include "dogen/utility/log/logger.hpp"
#include "dogen/formatters/types/comment_formatter.hpp"
#include "dogen/formatters/types/indent_filter.hpp"
#include "dogen/cpp/types/formatters/io/traits.hpp"
#include "dogen/cpp/types/formatters/types/traits.hpp"
#include "dogen/cpp/types/formatters/serialization/traits.hpp"
#include "dogen/cpp/types/formatters/formatting_error.hpp"
#include "dogen/cpp/types/formatters/io/helper_methods_formatter.hpp"
#include "dogen/cpp/types/formatters/entity_formatting_assistant.hpp"

namespace {

using namespace dogen::utility::log;
static logger lg(logger_factory("cpp.formatters.entity_formatting_assistant"));

const std::string empty;
const std::string by_ref_text = "&";
const std::string void_keyword_text = "void";
const std::string final_keyword_text = "final ";
const std::string member_variable_postfix("_");

const bool start_on_first_line(true);
const bool use_documentation_tool_markup(true);
const bool last_line_is_blank(true);
const bool documenting_previous_identifier(true);

const std::string file_path_not_set(
    "File path for formatter is not set. Formatter: ");
const std::string header_guard_not_set(
    "Header guard for formatter is not set. Formatter: ");
const std::string formatter_properties_missing(
    "Could not find formatter properties for formatter: ");

}

namespace dogen {
namespace cpp {
namespace formatters {

std::string entity_formatting_assistant::
make_final_keyword_text(const formattables::class_info& c) {
    return c.is_final() ? final_keyword_text : empty;
}

std::string entity_formatting_assistant::
make_by_ref_text(const formattables::property_info& p) {
    return p.type().is_primitive() ? empty : by_ref_text;
}

std::string entity_formatting_assistant::
make_setter_return_type(const std::string& containing_type_name,
    const formattables::property_info& p) {
    std::ostringstream s;
    if (p.is_fluent())
        s << containing_type_name << by_ref_text;
    else
        s << void_keyword_text;

    return s.str();
}

entity_formatting_assistant::
entity_formatting_assistant(const formattables::entity& e,
    const dynamic::ownership_hierarchy& oh,
    const formatters::file_types ft) :
    entity_(e), ownership_hierarchy_(oh),
    formatter_properties_(obtain_formatter_properties(oh.formatter_name())),
    file_type_(ft) {

    dogen::formatters::indent_filter::push(filtering_stream_, 4);
    filtering_stream_.push(stream_);
    validate();
}

formattables::formatter_properties entity_formatting_assistant::
obtain_formatter_properties(const std::string& formatter_name) const {
    const auto& fn(formatter_name);
    const auto i(entity_.formatter_properties().find(fn));
    if (i == entity_.formatter_properties().end()) {
        BOOST_LOG_SEV(lg, error) << formatter_properties_missing << fn;
        BOOST_THROW_EXCEPTION(
            formatting_error(formatter_properties_missing + fn));
    }
    return i->second;
}

std::string entity_formatting_assistant::make_member_variable_name(
    const formattables::property_info& p) const {
    return p.name() + member_variable_postfix;
}

bool entity_formatting_assistant::
is_formatter_enabled(const std::string& formatter_name) const {
    const auto fp(obtain_formatter_properties(formatter_name));
    return fp.enabled();
}

bool entity_formatting_assistant::
is_facet_integrated(const std::string& facet_name) const {
    const auto& f(formatter_properties_.integrated_facets());
    const auto i(f.find(facet_name));
    return i != f.end();
}

bool entity_formatting_assistant::is_serialization_enabled() const {
    using formatters::serialization::traits;
    return is_formatter_enabled(traits::class_header_formatter_name());
}

bool entity_formatting_assistant::is_io_enabled() const {
    using formatters::io::traits;
    return is_formatter_enabled(traits::class_header_formatter_name());
}

bool entity_formatting_assistant::is_io_integrated() const {
    using formatters::io::traits;
    return is_facet_integrated(traits::facet_name());
}

bool entity_formatting_assistant::is_complete_constructor_disabled() const {
    return entity_.settings().aspect_settings().disable_complete_constructor();
}

void entity_formatting_assistant::validate() const {
    const auto& fn(ownership_hierarchy_.formatter_name());
    const auto& fp(formatter_properties_);
    if (fp.file_path().empty()) {
        BOOST_LOG_SEV(lg, error) << file_path_not_set << fn;
        BOOST_THROW_EXCEPTION(formatting_error(file_path_not_set + fn));
    }

    if (file_type_ == file_types::cpp_header) {
        if (!fp.header_guard() || fp.header_guard()->empty()) {
            BOOST_LOG_SEV(lg, error) << header_guard_not_set << fn;
            BOOST_THROW_EXCEPTION(formatting_error(header_guard_not_set + fn));
        }
    }
}

dogen::formatters::cpp::scoped_boilerplate_formatter
entity_formatting_assistant::make_scoped_boilerplate_formatter() {
    const auto& fp(formatter_properties_);
    const auto gs(entity_.settings().general_settings());
    return dogen::formatters::cpp::scoped_boilerplate_formatter(
        stream(), gs, fp.inclusion_dependencies(),
        fp.header_guard() ? *fp.header_guard() : empty);
}

dogen::formatters::cpp::scoped_namespace_formatter
entity_formatting_assistant::make_scoped_namespace_formatter() {
    return dogen::formatters::cpp::scoped_namespace_formatter(
        stream(), entity_.namespaces(),
        false/*create_anonymous_namespace*/,
        true/*add_new_line*/);
}

dogen::formatters::sequence_formatter entity_formatting_assistant::
make_sequence_formatter(const unsigned int sequence_size,
    const std::string& element_separator) const {
    dogen::formatters::sequence_formatter r(sequence_size, element_separator);
    return r;
}

nested_type_formatting_assistant entity_formatting_assistant::
make_nested_type_formatting_assistant() {
    return nested_type_formatting_assistant(stream());
}

std::ostream& entity_formatting_assistant::stream() {
    return filtering_stream_;
}

dogen::formatters::file entity_formatting_assistant::
make_file(const bool overwrite) const {
    dogen::formatters::file r;
    r.content(stream_.str());
    r.path(formatter_properties_.file_path());
    r.overwrite(overwrite);
    return r;
}

void entity_formatting_assistant::comment(const std::string& c) {
    if (c.empty())
        return;

    dogen::formatters::comment_formatter f(
        !start_on_first_line,
        use_documentation_tool_markup,
        !documenting_previous_identifier,
        dogen::formatters::comment_styles::c_style,
        !last_line_is_blank);
    f.format(stream(), c);
}

void entity_formatting_assistant::
comment_start_method_group(const std::string& documentation,
    const bool add_comment_blocks) {
    if (documentation.empty())
        return;

    {
        dogen::formatters::positive_indenter_scope pis(stream());
        dogen::formatters::comment_formatter f(
            !start_on_first_line,
            use_documentation_tool_markup,
            !documenting_previous_identifier,
            dogen::formatters::comment_styles::c_style,
            !last_line_is_blank);

        f.format(stream(), documentation);
        if (add_comment_blocks) {
            f.format_doxygen_start_block(stream(), documentation);
            stream() << std::endl;
        }
    }
}

void entity_formatting_assistant::
comment_end_method_group(const std::string& documentation,
    const bool add_comment_blocks) {
    if (documentation.empty())
        return;

    {
        dogen::formatters::positive_indenter_scope pis(stream());
        dogen::formatters::comment_formatter f(
            start_on_first_line,
            use_documentation_tool_markup,
            !documenting_previous_identifier,
            dogen::formatters::comment_styles::c_style,
            !last_line_is_blank);

        if (add_comment_blocks) {
            f.format_doxygen_end_block(stream(), documentation);
            stream() << std::endl;
        }
    }
}

std::string entity_formatting_assistant::comment_inline(
    const std::string& c) const {
    if (c.empty())
        return empty;

    std::ostringstream s;
    s << " ";
    dogen::formatters::comment_formatter f(
        start_on_first_line,
        use_documentation_tool_markup,
        documenting_previous_identifier,
        dogen::formatters::comment_styles::cpp_style,
        !last_line_is_blank);

    f.format(s, c);
    return s.str();
}

void entity_formatting_assistant::add_helper_methods() {
    const auto ci(dynamic_cast<const formattables::class_info*>(&entity_));
    if (!ci) {
        BOOST_LOG_SEV(lg, debug) << "Entity does not require helper methods: "
                                 << ci->name();
        return;
    }

    BOOST_LOG_SEV(lg, debug) << "Processing entity: " << ci->name();

    using formatters::types::traits;
    const auto& cifn(traits::class_implementation_formatter_name());
    const auto& fn(ownership_hierarchy_.formatter_name());
    const bool is_types_class_implementation(fn == cifn);
    const bool has_io(is_io_enabled() && (is_io_integrated() ||
            ci->is_parent() || !ci->parents().empty()));

    if (is_types_class_implementation && has_io) {
        BOOST_LOG_SEV(lg, debug) << " Creating helper methods.";
        io::helper_methods_formatter f(ci->properties());
        f.format(stream());
    } else
        BOOST_LOG_SEV(lg, debug) << " Helper methods not required."
                                 << " is types class implementation: "
                                 << is_types_class_implementation
                                 << " has io: " << has_io;
}

} } }