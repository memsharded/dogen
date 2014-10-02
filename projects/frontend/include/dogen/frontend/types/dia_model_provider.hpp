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
#ifndef DOGEN_FRONTEND_TYPES_DIA_MODEL_PROVIDER_HPP
#define DOGEN_FRONTEND_TYPES_DIA_MODEL_PROVIDER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include "dogen/frontend/types/model_provider_interface.hpp"

namespace dogen {
namespace frontend {

/**
 * @brief Provider that reads Dia diagrams and generates partial SML
 * models.
 */
class dia_model_provider final : public model_provider_interface {
public:
    dia_model_provider() = default;
    dia_model_provider(const dia_model_provider&) = delete;
    dia_model_provider(dia_model_provider&&) = default;
    virtual ~dia_model_provider() noexcept;

public:
    std::string id() const override;
    std::list<std::string> supported_extensions() const override;
    sml::model
        provide(const input_descriptor& d, const provider_settings& s) override;
};

} }

#endif
