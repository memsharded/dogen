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
#ifndef DOGEN_QUILT_CPP_TYPES_SETTINGS_INCLUSION_DIRECTIVES_SETTINGS_HPP
#define DOGEN_QUILT_CPP_TYPES_SETTINGS_INCLUSION_DIRECTIVES_SETTINGS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <string>
#include <algorithm>
#include <unordered_map>
#include "dogen/quilt.cpp/types/settings/inclusion_directive_settings.hpp"
#include "dogen/quilt.cpp/serialization/settings/inclusion_directives_settings_fwd_ser.hpp"

namespace dogen {
namespace quilt {
namespace cpp {
namespace settings {

class inclusion_directives_settings final {
public:
    inclusion_directives_settings(const inclusion_directives_settings&) = default;
    inclusion_directives_settings(inclusion_directives_settings&&) = default;
    ~inclusion_directives_settings() = default;

public:
    inclusion_directives_settings();

public:
    inclusion_directives_settings(
        const bool inclusion_required,
        const std::unordered_map<std::string, dogen::quilt::cpp::settings::inclusion_directive_settings>& inclusion_directive_settings);

private:
    template<typename Archive>
    friend void boost::serialization::save(Archive& ar, const inclusion_directives_settings& v, unsigned int version);

    template<typename Archive>
    friend void boost::serialization::load(Archive& ar, inclusion_directives_settings& v, unsigned int version);

public:
    bool inclusion_required() const;
    void inclusion_required(const bool v);

    const std::unordered_map<std::string, dogen::quilt::cpp::settings::inclusion_directive_settings>& inclusion_directive_settings() const;
    std::unordered_map<std::string, dogen::quilt::cpp::settings::inclusion_directive_settings>& inclusion_directive_settings();
    void inclusion_directive_settings(const std::unordered_map<std::string, dogen::quilt::cpp::settings::inclusion_directive_settings>& v);
    void inclusion_directive_settings(const std::unordered_map<std::string, dogen::quilt::cpp::settings::inclusion_directive_settings>&& v);

public:
    bool operator==(const inclusion_directives_settings& rhs) const;
    bool operator!=(const inclusion_directives_settings& rhs) const {
        return !this->operator==(rhs);
    }

public:
    void swap(inclusion_directives_settings& other) noexcept;
    inclusion_directives_settings& operator=(inclusion_directives_settings other);

private:
    bool inclusion_required_;
    std::unordered_map<std::string, dogen::quilt::cpp::settings::inclusion_directive_settings> inclusion_directive_settings_;
};

} } } }

namespace std {

template<>
inline void swap(
    dogen::quilt::cpp::settings::inclusion_directives_settings& lhs,
    dogen::quilt::cpp::settings::inclusion_directives_settings& rhs) {
    lhs.swap(rhs);
}

}

#endif
