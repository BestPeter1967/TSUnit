####################################################################################
# @(#)File: CMakeLists.txt
# Created: 2022-02-01
# --------------------------------------------------------------------------
#  (c)1982-2022 Tangerine-Software
#
#       Hans-Peter Beständig
#       Kühbachstr. 8
#       81543 München
#       GERMANY
#
#       mailto:hdusel@tangerine-soft.de
#       http://hdusel.tangerine-soft.de
# --------------------------------------------------------------------------
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
####################################################################################
if( COMMAND CMAKE_MINIMUM_REQUIRED )
    CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
endif( COMMAND CMAKE_MINIMUM_REQUIRED )

cmake_policy(SET CMP0076 NEW)

####################################################################################
# The name of the project
####################################################################################
project(TSUnit)
set(CMAKE_CXX_STANDARD 14)
enable_language(C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

####################################################################################
# The libs to build to.
####################################################################################


####################################################################################
# 1 ) TSUnit with main
####################################################################################
add_library(TSUnit STATIC "")

target_sources(${PROJECT_NAME}
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/TSUnit.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TSUnitTestAddOns.cpp"
PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/TSUnit.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TSUnitTestAddOns.hpp"
)

target_include_directories(TSUnit
PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_compile_definitions(TSUnit
PRIVATE
    UT_USE_COLORED_OUTPUT
)

####################################################################################
# 2 ) TSUnit as Library
####################################################################################
add_library(TSUnitAsLib STATIC "")

target_sources(TSUnitAsLib
PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/TSUnit.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TSUnitTestAddOns.cpp"
PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/TSUnit.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TSUnitTestAddOns.hpp"
)

target_include_directories(TSUnitAsLib
PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_compile_definitions(TSUnitAsLib
PRIVATE
    UT_USE_COLORED_OUTPUT
    UNITTEST_AS_LIBCALL
)

macro(TESTCASE name)
    enable_testing()
    add_executable(UT_${name} ${CMAKE_CURRENT_SOURCE_DIR}/UT_${name}.cpp)
    target_link_libraries(UT_${name} PUBLIC ${PROJECT_NAME})
    add_test(NAME UT_${name} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/UT_${name})
endmacro()
macro(TESTCASE_AS_LIB name)
    enable_testing()
    add_executable(UT_${name} ${CMAKE_CURRENT_SOURCE_DIR}/UT_${name}.cpp)
    target_link_libraries(UT_${name} PUBLIC TSUnitAsLib)
    add_test(NAME UT_${name} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/UT_${name})
endmacro()

####################################################################################
# Add support for Tests
####################################################################################
enable_testing()

####################################################################################
# Additional Directories to deal with...
####################################################################################
add_subdirectory(unittests)
