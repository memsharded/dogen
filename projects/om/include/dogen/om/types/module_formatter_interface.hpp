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
#ifndef DOGEN_OM_TYPES_MODULE_FORMATTER_INTERFACE_HPP
#define DOGEN_OM_TYPES_MODULE_FORMATTER_INTERFACE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <iosfwd>
#include "dogen/sml/types/model.hpp"
#include "dogen/sml/types/module.hpp"
#include "dogen/om/types/file.hpp"
#include "dogen/om/types/licence.hpp"
#include "dogen/om/types/modeline.hpp"

namespace dogen {
namespace om {

/**
 * @brief Formatter that is responsible for the formtatting of a
 * single type.
 */
class module_formatter_interface {
public:
    module_formatter_interface() = default;
    module_formatter_interface(const module_formatter_interface&) = default;
    module_formatter_interface(module_formatter_interface&&) = default;

public:
    virtual ~module_formatter_interface() noexcept { }

public:
    /**
     * @brief Format the type according to some grammar into the
     * stream.
     */
    virtual file format(const sml::module& module, const licence& l,
        const modeline& modeline, const std::string& marker) const = 0;
};

} }

#endif