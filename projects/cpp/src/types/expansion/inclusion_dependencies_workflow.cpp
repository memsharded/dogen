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
#include <boost/throw_exception.hpp>
#include "dogen/utility/log/logger.hpp"
#include "dogen/utility/io/unordered_map_io.hpp"
#include "dogen/utility/io/pair_io.hpp"
#include "dogen/utility/io/list_io.hpp"
#include "dogen/sml/io/qname_io.hpp"
#include "dogen/sml/types/string_converter.hpp"
#include "dogen/sml/types/all_model_items_traversal.hpp"
#include "dogen/cpp/types/workflow_error.hpp"
#include "dogen/cpp/types/settings/inclusion_directives_factory.hpp"
#include "dogen/cpp/types/expansion/inclusion_dependencies_factory.hpp"
#include "dogen/cpp/types/expansion/inclusion_dependencies_workflow.hpp"

namespace {

using namespace dogen::utility::log;
static logger lg(logger_factory(
        "cpp.expansion.inclusion_dependencies_workflow"));

const std::string duplicate_qname("Duplicate qname: ");
const std::string model_module_not_found("Model module not found for model: ");

}

namespace dogen {
namespace cpp {
namespace expansion {

/**
 * @brief Generates all inclusion directives.
 */
class inclusion_directives_generator {
public:
    inclusion_directives_generator(const dynamic::schema::repository& rp,
        const formatters::container& fc) : factory_(rp, fc) { }

private:
    /**
     * @brief Generates all of the inclusion dependencies for the
     * formatters and qualified name.
     */
    template<typename ExtensibleAndNameable>
    void generate(const ExtensibleAndNameable& e) {
        const auto directives(factory_.make(e.extensions()));
        const auto pair(result_.insert(std::make_pair(e.name(), directives)));
        const bool inserted(pair.second);
        if (!inserted) {
            const auto n(sml::string_converter::convert(e.name()));
            BOOST_LOG_SEV(lg, error) << duplicate_qname << n;
            BOOST_THROW_EXCEPTION(workflow_error(duplicate_qname + n));
        }
    }

public:
    void operator()(const dogen::sml::object& o) { generate(o); }
    void operator()(const dogen::sml::enumeration& e) { generate(e); }
    void operator()(const dogen::sml::primitive& p) { generate(p); }
    void operator()(const dogen::sml::module& m) { generate(m); }
    void operator()(const dogen::sml::concept& c) { generate(c); }

public:
    const std::unordered_map<
    sml::qname,
    std::unordered_map<std::string, std::string>
    >& result() const;

private:
    const settings::inclusion_directives_factory factory_;
    std::unordered_map<
        sml::qname,
        std::unordered_map<std::string, std::string>
        > result_;
};

const std::unordered_map<
    sml::qname,
    std::unordered_map<std::string, std::string>
    >&
inclusion_directives_generator::result() const {
    return result_;
}

/**
 * @brief Generates all inclusion dependencies.
 */
class inclusion_dependencies_generator {
public:
    inclusion_dependencies_generator(const dynamic::schema::repository& rp,
        const container& c,
        const std::unordered_map<
            sml::qname,
            std::unordered_map<std::string, std::string>
            >& inclusion_directives) : factory_(rp, c, inclusion_directives) { }

private:
    /**
     * @brief Generates all of the inclusion dependencies for the
     * formatters and qualified name.
     */
    template<typename Entity>
    void generate(const Entity& e) {
        const auto deps(factory_.make(e));
        const auto pair(result_.insert(std::make_pair(e.name(), deps)));
        const bool inserted(pair.second);
        if (!inserted) {
            const auto n(sml::string_converter::convert(e.name()));
            BOOST_LOG_SEV(lg, error) << duplicate_qname << n;
            BOOST_THROW_EXCEPTION(workflow_error(duplicate_qname + n));
        }
    }

public:
    void operator()(const dogen::sml::object& o) { generate(o); }
    void operator()(const dogen::sml::enumeration& e) { generate(e); }
    void operator()(const dogen::sml::primitive& p) { generate(p); }
    void operator()(const dogen::sml::module& m) { generate(m); }
    void operator()(const dogen::sml::concept& c) { generate(c); }

public:
    const std::unordered_map<
    sml::qname,
    std::unordered_map<std::string, std::list<std::string> >
    >& result() const;

private:
    const inclusion_dependencies_factory factory_;
    std::unordered_map<
        sml::qname,
        std::unordered_map<std::string, std::list<std::string> >
        > result_;
};

const std::unordered_map<
    sml::qname,
    std::unordered_map<std::string, std::list<std::string> >
    >&
inclusion_dependencies_generator::result() const {
    return result_;
}

inclusion_dependencies_workflow::inclusion_dependencies_workflow(
    const formatters::container& c) : container_(c) {}

std::unordered_map<
    sml::qname,
    std::unordered_map<std::string, std::string>
    >
inclusion_dependencies_workflow::
obtain_inclusion_directives_activity(const dynamic::schema::repository& rp,
    const sml::model& m) const {
    BOOST_LOG_SEV(lg, debug) << "Started obtaining inclusion directives.";

    inclusion_directives_generator g(rp, container_);
    sml::all_model_items_traversal(m, g);

    const auto& r(g.result());
    BOOST_LOG_SEV(lg, debug) << "Finished obtaining inclusion directives:"
                             << r;
    return r;
}

void inclusion_dependencies_workflow::
initialise_registrar_activity(registrar& rg) const {
    BOOST_LOG_SEV(lg, debug) << "Started registering all providers.";
    for (const auto f : container_.all_formatters()) {
        BOOST_LOG_SEV(lg, debug) << "Registered: "
                                 << f->ownership_hierarchy().formatter_name();
        f->register_inclusion_dependencies_provider(rg);
    }
    BOOST_LOG_SEV(lg, debug) << "Finished registering all providers.";
}

std::unordered_map<
    sml::qname,
    std::unordered_map<std::string, std::list<std::string> >
    >
inclusion_dependencies_workflow::
obtain_inclusion_dependencies_activity(
    const dynamic::schema::repository& rp, const container& c,
    const std::unordered_map<
        sml::qname,
        std::unordered_map<std::string, std::string>
        >& inclusion_directives, const sml::model& m) const {
    BOOST_LOG_SEV(lg, debug) << "Started obtaining inclusion dependencies.";

    inclusion_dependencies_generator g(rp, c, inclusion_directives);
    sml::all_model_items_traversal(m, g);

    const auto& r(g.result());
    BOOST_LOG_SEV(lg, debug) << "Finished obtaining inclusion dependencies:"
                             << r;
    return r;
}

std::unordered_map<
    sml::qname,
    std::unordered_map<std::string, std::list<std::string> >
    >
inclusion_dependencies_workflow::
execute(const dynamic::schema::repository& rp, const sml::model& m) const {
    registrar rg;
    initialise_registrar_activity(rg);
    const auto c(rg.container());
    const auto id(obtain_inclusion_directives_activity(rp, m));
    const auto r(obtain_inclusion_dependencies_activity(rp, c, id, m));
    return r;
}

} } }
