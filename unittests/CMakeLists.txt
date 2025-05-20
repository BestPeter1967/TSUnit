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
cmake_policy(SET CMP0079 NEW)

####################################################################################
# The name of the project
####################################################################################
set(CMAKE_CXX_STANDARD 11)
enable_language(C CXX)

include_directories("${PROJECT_SOURCE_DIR}/../")

####################################################################################
# The executeable(s) to build to.
####################################################################################
TESTCASE(TSUnit)
TESTCASE(TSUnitTestAddOns)
TESTCASE_AS_LIB(TSUnit_AsCustomTests)

####################################################################################
# Add support for Tests
####################################################################################
enable_testing()
