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
#include <memory>
#include <algorithm>
#include <forward_list>
#include "dogen/utility/log/logger.hpp"
#include "dogen/utility/io/memory_io.hpp"
#include "dogen/utility/io/forward_list_io.hpp"
#include "dogen/yarn/types/element_visitor.hpp"
#include "dogen/quilt.cpp/types/settings/path_settings_factory.hpp"
#include "dogen/quilt.cpp/types/formattables/factory.hpp"
#include "dogen/quilt.cpp/types/formattables/transformer.hpp"
#include "dogen/quilt.cpp/io/formattables/formattable_io.hpp"
#include "dogen/quilt.cpp/types/formattables/path_derivatives_repository_factory.hpp"
#include "dogen/quilt.cpp/types/formattables/workflow.hpp"

namespace {

const std::string id("quilt.cpp.formattables.workflow");

using namespace dogen::utility::log;
static logger lg(logger_factory(id));

}

namespace dogen {
namespace quilt {
namespace cpp {
namespace formattables {

namespace {

/**
 * @brief Generates the formattables.
 */
class generator final : public yarn::element_visitor {
public:
    explicit generator(const transformer& t) : transformer_(t) { }

private:
    template<typename Transformable>
    void transform(const Transformable& t) {
        if (t.generation_type() != yarn::generation_types::no_generation) {
            auto f(transformer_.transform(t));
            result_.splice_after(result_.before_begin(), f);
        }
    }

public:
    using yarn::element_visitor::visit;
    void visit(const dogen::yarn::enumeration& e) { transform(e); }
    void visit(const dogen::yarn::object& o) { transform(o); }
    void visit(const dogen::yarn::exception& e) { transform(e); }

public:
    const std::forward_list<std::shared_ptr<formattables::formattable> >&
    result() { return result_; }

private:
    std::forward_list<std::shared_ptr<formattables::formattable> > result_;
    const transformer& transformer_;
};

}

std::unordered_map<std::string, settings::path_settings>
workflow::create_path_settings_activity(const dynamic::repository& rp,
    const dynamic::object& root_object,
    const formatters::container& fc) const {

    BOOST_LOG_SEV(lg, debug) << "Creating path settings for root object.";
    settings::path_settings_factory f(rp, fc.all_external_formatters());
    const auto r(f.make(root_object));
    BOOST_LOG_SEV(lg, debug) << "Created path settings for root object.";
    return r;
}

path_derivatives_repository workflow::
create_path_derivatives_repository(const config::cpp_options& opts,
    const std::unordered_map<std::string, settings::path_settings>& ps,
    const yarn::model& m) const {
    path_derivatives_repository_factory f;
    return f.make(opts, ps, m);
}

formatter_properties_repository workflow::
create_formatter_properties(const dynamic::repository& rp,
    const dynamic::object& root_object,
    const settings::bundle_repository& brp,
    const path_derivatives_repository& pdrp,
    const formatters::container& fc,
    const yarn::model& m) const {

    formatter_properties_repository_factory f;
    return f.make(rp, root_object, brp, pdrp, fc, m);
}

std::forward_list<std::shared_ptr<formattables::formattable> >
workflow::from_transformer_activity(const yarn::model& m) const {
    BOOST_LOG_SEV(lg, debug) << "Transforming formattables.";

    const transformer t(m);
    generator g(t);
    for (const auto& pair : m.elements()) {
        const auto& e(*pair.second);
        e.accept(g);
    }

    BOOST_LOG_SEV(lg, debug) << "Finished transforming formattables.";
    return g.result();
}

std::forward_list<std::shared_ptr<formattables::formattable> >
workflow::from_factory_activity(const config::cpp_options& opts,
    const dynamic::object& root_object,
    const dogen::formatters::general_settings_factory& gsf,
    settings::bundle_repository& brp,
    const std::unordered_map<std::string, settings::path_settings>& ps,
    const formattables::path_derivatives_repository& pdrp,
    formatter_properties_repository& fprp,
    const formatters::container& fc,
    const yarn::model& m) const {

    const auto& formatters(fc.all_formatters());
    std::forward_list<std::shared_ptr<formattables::formattable> > r;
    factory f;
    const auto& ro(root_object);
    const auto ri(f.make_registrar_info(opts, brp, ps, fprp, m));
    if (ri)
        r.push_front(ri);

    r.splice_after(r.before_begin(),
        f.make_includers(opts, ro, gsf, brp, ps, pdrp, formatters, fprp, m));
    r.splice_after(r.before_begin(),
        f.make_cmakelists(opts, ro, gsf, ps, fprp, m));

    const auto oi(f.make_odb_options(opts, ro, gsf, ps, fprp, m));
    if (oi)
        r.push_front(oi);

    BOOST_LOG_SEV(lg, debug) << "Factory formattables: " << r;
    return r;
}

std::pair<
    formattables::formatter_properties_repository,
    std::forward_list<std::shared_ptr<formattables::formattable> >
>
workflow::execute(const config::cpp_options& opts,
    const dynamic::repository& rp,
    const dynamic::object& root_object,
    const dogen::formatters::general_settings_factory& gsf,
    const formatters::container& fc,
    settings::bundle_repository& brp,
    const yarn::model& m) const {
    BOOST_LOG_SEV(lg, debug) << "Started creating formattables.";

    const auto& ro(root_object);
    const auto ps(create_path_settings_activity(rp, ro, fc));
    const auto pdrp(create_path_derivatives_repository(opts, ps, m));

    auto fprp(create_formatter_properties(rp, ro, brp, pdrp, fc, m));

    auto formattables(from_transformer_activity(m));
    formattables.splice_after(formattables.before_begin(),
        from_factory_activity(opts, ro, gsf, brp, ps, pdrp, fprp, fc, m));
    BOOST_LOG_SEV(lg, debug) << "Formattables: " << formattables;

    BOOST_LOG_SEV(lg, debug) << "Finished creating formattables.";

    return std::make_pair(fprp, formattables);
}

} } } }
