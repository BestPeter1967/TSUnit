################################################################################
# @(#)File: Makefile
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
################################################################################

################################################################################
# The location of TSUnit.hpp and TSUnit.cpp
################################################################################
TSUNIT_BASE_PATH=../../

################################################################################
# Mention all Unittest files HERE!
################################################################################
UNITTEST_FILES = \
	unittests/UT_CPoorMansBlockAlloc.cpp

TS_UNIT_HDR_FILES =	$(TSUNIT_BASE_PATH)/TSUnit.hpp
TS_UNIT_SRC_FILES =	$(TSUNIT_BASE_PATH)/TSUnit.cpp

################################################################################
# The Rules
################################################################################
run_unittests : unittests_exe
	./unittests_exe

unittests_exe : $(UNITTEST_FILES) libunittest.a
	g++ -std=c++11 -DUT_USE_COLORED_OUTPUT -I $(TSUNIT_BASE_PATH) -I ./ $(UNITTEST_FILES) -L ./ -lunittest -o $@

unittest_lib.o : $(TS_UNIT_HDR_FILES) $(TS_UNIT_SRC_FILES) Makefile
	g++ -O3 -DUT_USE_COLORED_OUTPUT -c -std=c++11 $(TS_UNIT_SRC_FILES) -o $@

libunittest.a : unittest_lib.o
	ar -r $@ $<

clean:
	rm -f unittest_lib.o libunittest.a unittests_exe
