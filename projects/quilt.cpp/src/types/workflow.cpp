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
#include <boost/throw_exception.hpp>
#include <boost/algorithm/string/join.hpp>
#include "dogen/utility/log/logger.hpp"
#include "dogen/utility/filesystem/path.hpp"
#include "dogen/formatters/types/hydration_workflow.hpp"
#include "dogen/dynamic/types/workflow.hpp"
#include "dogen/quilt.cpp/types/formatters/workflow.hpp"
#include "dogen/quilt.cpp/types/formattables/workflow.hpp"
#include "dogen/quilt.cpp/types/settings/directory_names_settings_factory.hpp"
#include "dogen/quilt.cpp/types/settings/helper_settings_repository_factory.hpp"
#include "dogen/quilt.cpp/types/settings/bundle_repository_factory.hpp"
#include "dogen/quilt.cpp/types/fabric/workflow.hpp"
#include "dogen/quilt.cpp/types/workflow_error.hpp"
#include "dogen/quilt.cpp/types/workflow.hpp"

namespace {

const std::string id("quilt.cpp.workflow");

using namespace dogen::utility::log;
static logger lg(logger_factory(id));

const std::string dot(".");
const std::string model_module_not_found("Model module not found for model: ");

}

namespace dogen {
namespace quilt {
namespace cpp {

workflow::~workflow() noexcept { }

dynamic::object workflow::obtain_root_object(
    const yarn::model& m) const {
    BOOST_LOG_SEV(lg, debug) << "Obtaining model's root object.";

    const auto i(m.elements().find(m.name().qualified()));
    if (i == m.elements().end()) {
        const auto qn(m.name().qualified());
        BOOST_LOG_SEV(lg, error) << model_module_not_found << qn;
        BOOST_THROW_EXCEPTION(workflow_error(model_module_not_found + qn));
    }

    BOOST_LOG_SEV(lg, debug) << "Obtained model's root object.";
    return i->second->extensions();
}

dogen::formatters::repository workflow::create_formatters_repository(
    const std::forward_list<boost::filesystem::path>& dirs) const {
    dogen::formatters::hydration_workflow hw;
    return hw.hydrate(dirs);
}

dogen::formatters::general_settings_factory workflow::
create_general_settings_factory(const dogen::formatters::repository& frp,
    const dynamic::object& root_object) const {
    dogen::formatters::general_settings_factory r(frp, root_object);
    return r;
}

settings::opaque_settings_builder workflow::
create_opaque_settings_builder(const dynamic::repository& rp) const {
    settings::opaque_settings_builder r;
    r.setup(rp);
    r.validate();
    return r;
}

settings::bundle_repository workflow::create_bundle_repository(
    const dynamic::repository& rp, const dynamic::object& root_object,
    const dogen::formatters::general_settings_factory& gsf,
    const settings::opaque_settings_builder& osb,
    const yarn::model& m) const {
    settings::bundle_repository_factory f;
    return f.make(rp, root_object, gsf, osb, m);
}

settings::helper_settings_repository workflow::
create_helper_settings_repository(const dynamic::repository& rp,
    const yarn::model& m) const {
    settings::helper_settings_repository_factory f;
    return f.make(rp, m);
}

std::pair<
    formattables::formatter_properties_repository,
    std::forward_list<std::shared_ptr<formattables::formattable> >
>
workflow::create_formattables_activty(
    const config::cpp_options& opts,
    const dynamic::repository& srp,
    const dynamic::object& root_object,
    const dogen::formatters::general_settings_factory& gsf,
    const formatters::container& fc,
    settings::bundle_repository& brp,
    const yarn::model& m) const {

    formattables::workflow fw;
    return fw.execute(opts, srp, root_object, gsf, fc, brp, m);
}

std::forward_list<boost::shared_ptr<yarn::element> >
workflow::obtain_enriched_yarn_model_activity(const yarn::model& m) const {
    fabric::workflow w;
    return w.execute(m);
}

std::forward_list<dogen::formatters::file>
workflow::format_activty(const settings::bundle_repository& brp,
    const settings::helper_settings_repository& hsrp,
    const formattables::formatter_properties_repository& fprp,
    const std::forward_list<
    std::shared_ptr<formattables::formattable>
    >& f) const {
    formatters::workflow w;
    return w.execute(brp, hsrp, fprp, f);
}

std::forward_list<dogen::formatters::file> workflow::
format_yarn_activity(const settings::bundle_repository& brp,
    const settings::helper_settings_repository& hsrp,
    const formattables::formatter_properties_repository& fprp,
    const std::forward_list<
    boost::shared_ptr<yarn::element> >& elements) const {
    formatters::workflow w;
    return w.execute(brp, hsrp, fprp, elements);
}

std::string workflow::name() const {
    return ::id;
}

std::forward_list<boost::filesystem::path>
workflow::managed_directories(const config::knitting_options& ko,
    const dynamic::repository& rp, const yarn::model& m) const {
    const auto ro(obtain_root_object(m));
    settings::directory_names_settings_factory f(rp);
    const auto dn(f.make(ro));
    const auto& mm(m.name().location().model_modules());
    const auto mn(boost::algorithm::join(mm, dot));
    std::forward_list<boost::filesystem::path> r;
    r.push_front(ko.cpp().project_directory_path() / mn);
    return r;
}

std::forward_list<dynamic::ownership_hierarchy>
workflow::ownership_hierarchy() const {
    using formatters::workflow;
    return workflow::registrar().ownership_hierarchy();
}

std::forward_list<dogen::formatters::file>
workflow::generate(const config::knitting_options& ko,
    const dynamic::repository& rp,
    const yarn::model& m) const {
    BOOST_LOG_SEV(lg, debug) << "Started C++ backend.";

    const auto dir(dogen::utility::filesystem::data_files_directory());
    const auto dirs(std::forward_list<boost::filesystem::path> { dir });
    const auto frp(create_formatters_repository(dirs));

    const auto ro(obtain_root_object(m));
    const auto gsf(create_general_settings_factory(frp, ro));

    const auto osb(create_opaque_settings_builder(rp));
    auto brp(create_bundle_repository(rp, ro, gsf, osb, m));
    auto hsrp(create_helper_settings_repository(rp, m));

    formatters::workflow::registrar().validate();
    const auto& fc(formatters::workflow::registrar().formatter_container());

    const auto& kcpp(ko.cpp());
    const auto pair(create_formattables_activty(kcpp, rp, ro, gsf, fc, brp, m));
    auto r(format_activty(brp, hsrp, pair.first, pair.second));

    const auto elements(obtain_enriched_yarn_model_activity(m));
    auto ye(format_yarn_activity(brp, hsrp, pair.first, elements));
    r.splice_after(r.before_begin(), ye);

    BOOST_LOG_SEV(lg, debug) << "Finished C++ backend.";
    return r;
}

} } }
