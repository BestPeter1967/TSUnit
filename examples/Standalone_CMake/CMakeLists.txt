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
project(MySuperDuperProduct)
set(CMAKE_CXX_STANDARD 11)
enable_language(C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

####################################################################################
# The executeable(s) to build to.
####################################################################################
add_executable(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME}
PRIVATE
    MyProductionCode.cpp
    CPoorMansBlockAlloc.cpp
PUBLIC
    CPoorMansBlockAlloc.hpp
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#target_compile_definitions(${PROJECT_NAME}
#PUBLIC
#)

####################################################################################
# Target specific flags.
####################################################################################
#target_link_libraries(${PROJECT_NAME} PRIVATE pthread)

####################################################################################
# Add support for Tests
####################################################################################
enable_testing()
add_subdirectory(unittests)
