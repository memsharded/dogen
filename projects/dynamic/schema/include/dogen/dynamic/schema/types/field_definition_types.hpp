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
#ifndef DOGEN_DYNAMIC_SCHEMA_TYPES_FIELD_DEFINITION_TYPES_HPP
#define DOGEN_DYNAMIC_SCHEMA_TYPES_FIELD_DEFINITION_TYPES_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

namespace dogen {
namespace dynamic {
namespace schema {

enum class field_definition_types : unsigned int {
    invalid = 0, ///< Represents an uninitialised enum
    instance = 1,
    global_template = 2, ///< This field applies to all models, facets and formatters.
    model_template = 3, ///< This field applies to all models.
    facet_template = 4, ///< This field applies to all facets.
    formatter_template = 5 ///< This field applies to all formatters.
};

} } }

#endif
