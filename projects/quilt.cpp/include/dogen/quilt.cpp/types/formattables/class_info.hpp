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
#ifndef DOGEN_QUILT_CPP_TYPES_FORMATTABLES_CLASS_INFO_HPP
#define DOGEN_QUILT_CPP_TYPES_FORMATTABLES_CLASS_INFO_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <list>
#include <iosfwd>
#include <string>
#include <algorithm>
#include "dogen/yarn/types/generation_types.hpp"
#include "dogen/quilt.cpp/types/formattables/entity.hpp"
#include "dogen/quilt.cpp/types/formattables/parent_info.hpp"
#include "dogen/quilt.cpp/types/formattables/property_info.hpp"
#include "dogen/quilt.cpp/serialization/formattables/class_info_fwd_ser.hpp"

namespace dogen {
namespace quilt {
namespace cpp {
namespace formattables {

/**
 * @brief Represents a C++ class.
 */
class class_info final : public dogen::quilt::cpp::formattables::entity {
public:
    class_info(const class_info&) = default;
    class_info(class_info&&) = default;

public:
    class_info();

    virtual ~class_info() noexcept { }

public:
    class_info(
        const std::string& identity,
        const dogen::quilt::cpp::formattables::origin_types origin_type,
        const std::string& id,
        const std::string& name,
        const std::string& qualified_name,
        const std::string& documentation,
        const std::list<std::string>& namespaces,
        const std::list<dogen::quilt::cpp::formattables::property_info>& properties,
        const std::list<dogen::quilt::cpp::formattables::property_info>& all_properties,
        const bool requires_stream_manipulators,
        const bool requires_manual_move_constructor,
        const bool requires_manual_default_constructor,
        const std::list<dogen::quilt::cpp::formattables::parent_info>& parents,
        const bool is_parent,
        const std::string& root_parent_name,
        const std::string& root_parent_name_qualified,
        const std::list<std::string>& leaves,
        const bool is_visitable,
        const bool is_immutable,
        const bool is_root_parent_visitable,
        const dogen::yarn::generation_types generation_type,
        const bool is_final);

private:
    template<typename Archive>
    friend void boost::serialization::save(Archive& ar, const class_info& v, unsigned int version);

    template<typename Archive>
    friend void boost::serialization::load(Archive& ar, class_info& v, unsigned int version);

public:
    virtual void accept(const formattable_visitor& v) const override {
        v.visit(*this);
    }

    virtual void accept(formattable_visitor& v) const override {
        v.visit(*this);
    }

    virtual void accept(const formattable_visitor& v) override {
        v.visit(*this);
    }

    virtual void accept(formattable_visitor& v) override {
        v.visit(*this);
    }

public:
    void to_stream(std::ostream& s) const override;

public:
    /**
     * @brief Properties of the type itself (e.g. excludes inherited properties).
     */
    /**@{*/
    const std::list<dogen::quilt::cpp::formattables::property_info>& properties() const;
    std::list<dogen::quilt::cpp::formattables::property_info>& properties();
    void properties(const std::list<dogen::quilt::cpp::formattables::property_info>& v);
    void properties(const std::list<dogen::quilt::cpp::formattables::property_info>&& v);
    /**@}*/

    /**
     * @brief All properties of the type including inherited.
     */
    /**@{*/
    const std::list<dogen::quilt::cpp::formattables::property_info>& all_properties() const;
    std::list<dogen::quilt::cpp::formattables::property_info>& all_properties();
    void all_properties(const std::list<dogen::quilt::cpp::formattables::property_info>& v);
    void all_properties(const std::list<dogen::quilt::cpp::formattables::property_info>&& v);
    /**@}*/

    /**
     * @brief True if the class has member variables which require
     * changing the stream with manipulators, false otherwise.
     *
     * Examples are boolean variables, which should be outputted as
     * true or false and as such require boolalpha.
     */
    /**@{*/
    bool requires_stream_manipulators() const;
    void requires_stream_manipulators(const bool v);
    /**@}*/

    /**
     * @brief True if the class requires a manually generated move
     * constructor, false otherwise.
     */
    /**@{*/
    bool requires_manual_move_constructor() const;
    void requires_manual_move_constructor(const bool v);
    /**@}*/

    /**
     * @brief True if the class requires a manually generated default
     * constructor, false otherwise.
     */
    /**@{*/
    bool requires_manual_default_constructor() const;
    void requires_manual_default_constructor(const bool v);
    /**@}*/

    /**
     * @brief Parents for this class.
     */
    /**@{*/
    const std::list<dogen::quilt::cpp::formattables::parent_info>& parents() const;
    std::list<dogen::quilt::cpp::formattables::parent_info>& parents();
    void parents(const std::list<dogen::quilt::cpp::formattables::parent_info>& v);
    void parents(const std::list<dogen::quilt::cpp::formattables::parent_info>&& v);
    /**@}*/

    /**
     * @brief True if this class is the parent of one or more classes,
     * false otherwise.
     */
    /**@{*/
    bool is_parent() const;
    void is_parent(const bool v);
    /**@}*/

    /**
     * @brief Parent at the root of a class hierarchy, if any.
     */
    /**@{*/
    const std::string& root_parent_name() const;
    std::string& root_parent_name();
    void root_parent_name(const std::string& v);
    void root_parent_name(const std::string&& v);
    /**@}*/

    /**
     * @brief Root parent name as a fully qualified name.
     */
    /**@{*/
    const std::string& root_parent_name_qualified() const;
    std::string& root_parent_name_qualified();
    void root_parent_name_qualified(const std::string& v);
    void root_parent_name_qualified(const std::string&& v);
    /**@}*/

    /**
     * @brief If the class is abstract, list of concrete types that
     * descend from it. Names are in identifiable name format.
     */
    /**@{*/
    const std::list<std::string>& leaves() const;
    std::list<std::string>& leaves();
    void leaves(const std::list<std::string>& v);
    void leaves(const std::list<std::string>&& v);
    /**@}*/

    /**
     * @brief If true, the class has a visitor defined.
     */
    /**@{*/
    bool is_visitable() const;
    void is_visitable(const bool v);
    /**@}*/

    /**
     * @brief If true, the class has only getters.
     */
    /**@{*/
    bool is_immutable() const;
    void is_immutable(const bool v);
    /**@}*/

    /**
     * @brief If true, the class' root parent has a visitor defined.
     */
    /**@{*/
    bool is_root_parent_visitable() const;
    void is_root_parent_visitable(const bool v);
    /**@}*/

    /**
     * @brief Type of code generation to perform.
     */
    /**@{*/
    dogen::yarn::generation_types generation_type() const;
    void generation_type(const dogen::yarn::generation_types v);
    /**@}*/

    bool is_final() const;
    void is_final(const bool v);

public:
    bool operator==(const class_info& rhs) const;
    bool operator!=(const class_info& rhs) const {
        return !this->operator==(rhs);
    }

public:
    bool equals(const dogen::quilt::cpp::formattables::formattable& other) const override;

public:
    void swap(class_info& other) noexcept;
    class_info& operator=(class_info other);

private:
    std::list<dogen::quilt::cpp::formattables::property_info> properties_;
    std::list<dogen::quilt::cpp::formattables::property_info> all_properties_;
    bool requires_stream_manipulators_;
    bool requires_manual_move_constructor_;
    bool requires_manual_default_constructor_;
    std::list<dogen::quilt::cpp::formattables::parent_info> parents_;
    bool is_parent_;
    std::string root_parent_name_;
    std::string root_parent_name_qualified_;
    std::list<std::string> leaves_;
    bool is_visitable_;
    bool is_immutable_;
    bool is_root_parent_visitable_;
    dogen::yarn::generation_types generation_type_;
    bool is_final_;
};

} } } }

namespace std {

template<>
inline void swap(
    dogen::quilt::cpp::formattables::class_info& lhs,
    dogen::quilt::cpp::formattables::class_info& rhs) {
    lhs.swap(rhs);
}

}

#endif
