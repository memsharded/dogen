# -*- mode: cmake; tab-width: 4; indent-tabs-mode: nil -*-
#
# Copyright (C) 2012-2015 Marco Craveiro <marco.craveiro@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.
#
set(name "test_model_sanitizer")
set(binary_name "dogen.${name}.spec")
set(candidate_files "")
set(files "")
file(GLOB_RECURSE candidate_files RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

set(ignore_file false)
foreach(candidate_file ${candidate_files})
    if(NOT ODB_FOUND)
        if (candidate_file MATCHES "database")
            set(ignore_file true)
        endif()
    endif()

    if(NOT ignore_file)
        set(files ${files} ${candidate_file})
    endif()
    set(ignore_file false)
endforeach()

# we need to add these manually as the point of this test model
# is not to generate a make file.
file(GLOB_RECURSE disable_cmakelists_files ABSOLUTE
    "${CMAKE_SOURCE_DIR}/projects/test_models/disable_cmakelists/src/*.cpp")

add_executable(${name}_spec ${files} ${disable_cmakelists_files})
set_target_properties(${name}_spec PROPERTIES OUTPUT_NAME ${binary_name})

target_link_libraries(${name}_spec
    all_primitives
    classes_in_a_package
    classes_inout_package
    classes_without_package
    class_in_a_package
    class_without_package
    compressed
    disable_full_ctor
    disable_facet_folders
    enable_facet_domain
    enable_facet_hash
    enable_facet_io
    enable_facet_serialization
    stand_alone_class
    two_layers_with_objects
    trivial_inheritance
    trivial_association
    enumeration
    std_model
    boost_model
    stereotypes
    utility
    ${LIBXML2_LIBRARIES}
    ${CMAKE_REQUIRED_LIBRARIES}
    ${Boost_LIBRARIES})

# FIXME: link against database if feature is on?

set(command ${stage_bin_dir}/${binary_name} ${boost_test_parameters})

add_custom_target(run_${name}_spec
    COMMAND ${command}
    WORKING_DIRECTORY ${stage_bin_dir}
    DEPENDS ${name}_spec)
add_dependencies(run_all_specs run_${name}_spec)

add_boost_tests(${name} ${files})
install(TARGETS ${name}_spec RUNTIME DESTINATION bin COMPONENT tests)
