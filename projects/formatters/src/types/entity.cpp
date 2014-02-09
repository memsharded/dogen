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
#include "dogen/formatters/types/entity.hpp"

namespace dogen {
namespace formatters {

entity::~entity() noexcept { }

entity::entity(const dogen::formatters::annotation& annotation)
    : annotation_(annotation) { }

void entity::swap(entity& other) noexcept {
    using std::swap;
    swap(annotation_, other.annotation_);
}

bool entity::operator==(const entity& rhs) const {
    return annotation_ == rhs.annotation_;
}

entity& entity::operator=(entity other) {
    using std::swap;
    swap(*this, other);
    return *this;
}

const dogen::formatters::annotation& entity::annotation() const {
    return annotation_;
}

dogen::formatters::annotation& entity::annotation() {
    return annotation_;
}

void entity::annotation(const dogen::formatters::annotation& v) {
    annotation_ = v;
}

void entity::annotation(const dogen::formatters::annotation&& v) {
    annotation_ = std::move(v);
}

} }