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
#include <boost/lexical_cast.hpp>
#include <boost/throw_exception.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "dogen/utility/log/logger.hpp"
#include "dogen/stitch/types/building_error.hpp"
#include "dogen/stitch/types/builder.hpp"

namespace {

using namespace dogen::utility::log;
auto lg(logger_factory("stitch.builder"));

const std::string new_line("\n");
const std::string equals("=");

const std::string unsupported_state("State is not supported: ");
const std::string separator_not_found("Expected separator on kvp.");

}

namespace dogen {
namespace stitch {

builder::builder(const dynamic::schema::workflow& w)
    : schema_workflow_(w), current_state_(state_types::in_text_block) {}

void builder::start_standard_control_block() {
    BOOST_LOG_SEV(lg, debug) << "Starting standard control block";
    current_state_ = state_types::in_standard_control_block;
}

void builder::start_expression_control_block() {
    BOOST_LOG_SEV(lg, debug) << "Starting expression control block";
    current_state_ = state_types::in_expression_control_block;
}

void builder::start_directive() {
    BOOST_LOG_SEV(lg, debug) << "Starting directive";
    current_state_ = state_types::in_directive;
}

void builder::process_directive(const std::string& c) {
    const auto pos(c.find_first_of(equals));
    if (pos == std::string::npos) {
        BOOST_LOG_SEV(lg, error) << separator_not_found;
        BOOST_THROW_EXCEPTION(building_error(separator_not_found));
    }

    const auto key(c.substr(0, pos));
    const auto value(c.substr(pos + 1));
    directives_.push_back(std::make_pair(key, value));
}

void builder::add_segmented_content(const std::string& c) {
    switch (current_state_) {
    case state_types::in_standard_control_block: {
        segment sg;
        sg.content(c);
        sg.type(segment_types::scriptlet);
        current_line_->segments().push_back(sg);
        break;
    }
    case state_types::in_expression_control_block: {
        segment sg;
        sg.content(c);
        sg.type(segment_types::scriptlet);
        current_line_->segments().push_back(sg);
        break;
    }
    case state_types::in_directive: {
        process_directive(c);
        break;
    }
    case state_types::in_text_block: {
        segment sg;
        sg.content(c);
        sg.type(segment_types::text);
        current_line_->segments().push_back(sg);
        break;
    }
    default: {
        const auto int_state(static_cast<unsigned int>(current_state_));
        BOOST_LOG_SEV(lg, error) << unsupported_state << int_state;
        BOOST_THROW_EXCEPTION(building_error(unsupported_state +
                boost::lexical_cast<std::string>(int_state)));
    } }
    return;
}

void builder::add_content(const std::string& content) {
    BOOST_LOG_SEV(lg, debug) << "Adding content: " << content;

    if (!current_line_) {
        BOOST_LOG_SEV(lg, debug) << "No current line, creating.";
        current_line_ = line();
    }

    const bool has_new_line(boost::contains(content, new_line));
    if (!has_new_line) {
        BOOST_LOG_SEV(lg, debug) << "Content does not have new lines.";
        add_segmented_content(content);
        return;
    }

    BOOST_LOG_SEV(lg, debug) << "Content has new lines.";

    bool is_first(true);
    std::string input_line;
    std::istringstream is(content);
    while (std::getline(is, input_line)) {
        BOOST_LOG_SEV(lg, debug) << "Processing line: " << input_line;

        if (!is_first) {
            BOOST_LOG_SEV(lg, debug) << "Pushing line.";
            text_template_.lines().push_back(*current_line_);
            current_line_ = line();
        }
        is_first = false;
        add_segmented_content(input_line);
    }
}

void builder::end_control_block() {
    current_state_ = state_types::in_text_block;
}

text_template builder::build() {
    if (current_line_)
        text_template_.lines().push_back(*current_line_);

    using dynamic::schema::scope_types;
    const auto scope(scope_types::root_module);
    text_template_.extensions(schema_workflow_.execute(scope, directives_));

    return text_template_;
}

} }