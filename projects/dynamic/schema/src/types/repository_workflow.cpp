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
#include <unordered_set>
#include <boost/throw_exception.hpp>
#include "dogen/utility/log/logger.hpp"
#include "dogen/dynamic/schema/io/repository_io.hpp"
#include "dogen/dynamic/schema/types/workflow_error.hpp"
#include "dogen/dynamic/schema/types/hydration_workflow.hpp"
#include "dogen/dynamic/schema/types/repository_factory.hpp"
#include "dogen/dynamic/schema/types/repository_workflow.hpp"

namespace {

using namespace dogen::utility::log;
static logger lg(logger_factory("dynamic.schema.repository_workflow"));

}

namespace dogen {
namespace dynamic {
namespace schema {

std::list<field_definition> repository_workflow::hydrate_directories_activity(
    const std::forward_list<boost::filesystem::path>& dirs) const {
    hydration_workflow w;
    return w.hydrate(dirs);
}

repository repository_workflow::create_repository_activity(
    const std::list<field_definition>& fds) const {
    repository_factory f;
    return f.make(fds);
}

repository repository_workflow::execute(
    const std::forward_list<boost::filesystem::path>& dirs) const {
    BOOST_LOG_SEV(lg, debug) << "Generating repository.";

    const auto fds(hydrate_directories_activity(dirs));
    const auto r(create_repository_activity(fds));
    BOOST_LOG_SEV(lg, debug) << "Generated repository: " << r;
    return r;
}

} } }
