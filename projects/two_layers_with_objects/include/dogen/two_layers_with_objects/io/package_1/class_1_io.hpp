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
#ifndef DOGEN_TWO_LAYERS_WITH_OBJECTS_IO_PACKAGE_1_CLASS_1_IO_HPP
#define DOGEN_TWO_LAYERS_WITH_OBJECTS_IO_PACKAGE_1_CLASS_1_IO_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <iosfwd>
#include "dogen/two_layers_with_objects/domain/package_1/class_1.hpp"

namespace dogen {
namespace two_layers_with_objects {
namespace package_1 {

std::ostream&
operator<<(std::ostream& s,
     const dogen::two_layers_with_objects::package_1::class_1& v);

} } }

#endif
