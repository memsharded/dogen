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
#ifndef DOGEN_KNIT_TYPES_GENERATION_FAILURE_HPP
#define DOGEN_KNIT_TYPES_GENERATION_FAILURE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <boost/exception/info.hpp>
#include <string>

namespace dogen {
namespace knit {

class generation_failure : public virtual std::exception, public virtual boost::exception {
public:
    generation_failure() = default;
    ~generation_failure() noexcept = default;

public:
    generation_failure(const std::string& message) : message_(message) { }

public:
    const char* what() const noexcept { return(message_.c_str()); }

private:
    const std::string message_;
};

} }

#endif