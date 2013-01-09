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
#ifndef DOGEN_SML_TYPES_MODEL_HPP
#define DOGEN_SML_TYPES_MODEL_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <algorithm>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "dogen/sml/hash/qname_hash.hpp"
#include "dogen/sml/serialization/model_fwd_ser.hpp"
#include "dogen/sml/types/enumeration.hpp"
#include "dogen/sml/types/exception.hpp"
#include "dogen/sml/types/package.hpp"
#include "dogen/sml/types/pod.hpp"
#include "dogen/sml/types/primitive.hpp"
#include "dogen/sml/types/qname.hpp"
#include "dogen/sml/types/reference.hpp"

namespace dogen {
namespace sml {

/*
 * @brief Represents a domain model.
 */
class model final {
public:
    model(const model&) = default;
    model(model&&) = default;
    ~model() = default;

public:
    model();

public:
    model(
        const std::string& name,
        const std::unordered_map<dogen::sml::qname, dogen::sml::package>& packages,
        const std::unordered_map<dogen::sml::qname, dogen::sml::pod>& pods,
        const std::unordered_map<dogen::sml::qname, dogen::sml::primitive>& primitives,
        const std::unordered_map<dogen::sml::qname, dogen::sml::enumeration>& enumerations,
        const std::unordered_map<dogen::sml::qname, dogen::sml::exception>& exceptions,
        const std::list<std::string>& external_package_path,
        const std::string& schema_name,
        const bool is_system,
        const std::unordered_map<std::string, dogen::sml::reference>& dependencies,
        const std::unordered_set<dogen::sml::qname>& leaves);

private:
    template<typename Archive>
    friend void boost::serialization::save(Archive& ar, const model& v, unsigned int version);

    template<typename Archive>
    friend void boost::serialization::load(Archive& ar, model& v, unsigned int version);

public:
    /*
     * @brief Name of the domain model.
     *
     * Must not contain spaces or any special character as its used to
     * name folders in the file-system.
     */
    /**@{*/
    const std::string& name() const;
    std::string& name();
    void name(const std::string& v);
    void name(const std::string&& v);
    /**@}*/

    /*
     * @brief Packages contained in the model.
     */
    /**@{*/
    const std::unordered_map<dogen::sml::qname, dogen::sml::package>& packages() const;
    std::unordered_map<dogen::sml::qname, dogen::sml::package>& packages();
    void packages(const std::unordered_map<dogen::sml::qname, dogen::sml::package>& v);
    void packages(const std::unordered_map<dogen::sml::qname, dogen::sml::package>&& v);
    /**@}*/

    /*
     * @brief Pods contained in the model.
     */
    /**@{*/
    const std::unordered_map<dogen::sml::qname, dogen::sml::pod>& pods() const;
    std::unordered_map<dogen::sml::qname, dogen::sml::pod>& pods();
    void pods(const std::unordered_map<dogen::sml::qname, dogen::sml::pod>& v);
    void pods(const std::unordered_map<dogen::sml::qname, dogen::sml::pod>&& v);
    /**@}*/

    /*
     * @brief Primitives contained in the model.
     */
    /**@{*/
    const std::unordered_map<dogen::sml::qname, dogen::sml::primitive>& primitives() const;
    std::unordered_map<dogen::sml::qname, dogen::sml::primitive>& primitives();
    void primitives(const std::unordered_map<dogen::sml::qname, dogen::sml::primitive>& v);
    void primitives(const std::unordered_map<dogen::sml::qname, dogen::sml::primitive>&& v);
    /**@}*/

    /*
     * @brief Enumerations contained in the model.
     */
    /**@{*/
    const std::unordered_map<dogen::sml::qname, dogen::sml::enumeration>& enumerations() const;
    std::unordered_map<dogen::sml::qname, dogen::sml::enumeration>& enumerations();
    void enumerations(const std::unordered_map<dogen::sml::qname, dogen::sml::enumeration>& v);
    void enumerations(const std::unordered_map<dogen::sml::qname, dogen::sml::enumeration>&& v);
    /**@}*/

    /*
     * @brief Exceptions contained in the model.
     */
    /**@{*/
    const std::unordered_map<dogen::sml::qname, dogen::sml::exception>& exceptions() const;
    std::unordered_map<dogen::sml::qname, dogen::sml::exception>& exceptions();
    void exceptions(const std::unordered_map<dogen::sml::qname, dogen::sml::exception>& v);
    void exceptions(const std::unordered_map<dogen::sml::qname, dogen::sml::exception>&& v);
    /**@}*/

    /*
     * @brief Path of packages that contain this model.
     */
    /**@{*/
    const std::list<std::string>& external_package_path() const;
    std::list<std::string>& external_package_path();
    void external_package_path(const std::list<std::string>& v);
    void external_package_path(const std::list<std::string>&& v);
    /**@}*/

    /*
     * @brief Name of the database schema this model belongs to, if any.
     */
    /**@{*/
    const std::string& schema_name() const;
    std::string& schema_name();
    void schema_name(const std::string& v);
    void schema_name(const std::string&& v);
    /**@}*/

    /*
     * @brief If true, the model is a system model such as STL, boost, etc. If false, it is a user generated model.
     *
     * System models are a way to expose third party code into Dogen
     * so that we can make use of these types. They need to be
     * hand-coded in, because they require changes to the formatters.
     *
     * User models are regular Dogen models, normally made using Dia.
     */
    /**@{*/
    bool is_system() const;
    void is_system(const bool v);
    /**@}*/

    /*
     * @brief All other models that this model depends on - both system and user defined.
     */
    /**@{*/
    const std::unordered_map<std::string, dogen::sml::reference>& dependencies() const;
    std::unordered_map<std::string, dogen::sml::reference>& dependencies();
    void dependencies(const std::unordered_map<std::string, dogen::sml::reference>& v);
    void dependencies(const std::unordered_map<std::string, dogen::sml::reference>&& v);
    /**@}*/

    /*
     * @brief All leaf types in this model.
     *
     * Leaves are types concrete types which have a parent.
     */
    /**@{*/
    const std::unordered_set<dogen::sml::qname>& leaves() const;
    std::unordered_set<dogen::sml::qname>& leaves();
    void leaves(const std::unordered_set<dogen::sml::qname>& v);
    void leaves(const std::unordered_set<dogen::sml::qname>&& v);
    /**@}*/

public:
    bool operator==(const model& rhs) const;
    bool operator!=(const model& rhs) const {
        return !this->operator==(rhs);
    }

public:
    void swap(model& other) noexcept;
    model& operator=(model other);

private:
    std::string name_;
    std::unordered_map<dogen::sml::qname, dogen::sml::package> packages_;
    std::unordered_map<dogen::sml::qname, dogen::sml::pod> pods_;
    std::unordered_map<dogen::sml::qname, dogen::sml::primitive> primitives_;
    std::unordered_map<dogen::sml::qname, dogen::sml::enumeration> enumerations_;
    std::unordered_map<dogen::sml::qname, dogen::sml::exception> exceptions_;
    std::list<std::string> external_package_path_;
    std::string schema_name_;
    bool is_system_;
    std::unordered_map<std::string, dogen::sml::reference> dependencies_;
    std::unordered_set<dogen::sml::qname> leaves_;
};

} }

namespace std {

template<>
inline void swap(
    dogen::sml::model& lhs,
    dogen::sml::model& rhs) {
    lhs.swap(rhs);
}

}

#endif
