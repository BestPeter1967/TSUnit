/* ==========================================================================
 * @(#)File: UT_TSUnit_AsCustomTests.cpp.cpp
 * Created: 2022-06-19
 * --------------------------------------------------------------------------
 *  (c)1982-2022 Tangerine-Software
 *
 *       Hans-Peter Beständig
 *       Kühbachstr. 8
 *       81543 München
 *       GERMANY
 *
 *       mailto:hdusel@tangerine-soft.de
 *       http://hdusel.tangerine-soft.de
 * --------------------------------------------------------------------------
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * ========================================================================== */
#include "TSUnit.hpp"

TSUNIT_TEST(Standardtests, testForRepetitiveTests)
{
    static tsunit::Statistics& stats = tsunit::totalStatistics();

    UT_EXPECT_EQ(stats.runTestsCnt(), 1);

    UT_EXPECT_EQ(3, 3);
    UT_EXPECT_NE(3, 4);
    UT_EXPECT_TRUE(true);
    UT_EXPECT_FALSE(false);
}

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cassert>

/* ========================================================================== *
 * Main entry
 * ========================================================================== */
int main(int argc, char* argv[])
{
    int rc1 = tsunit::runUnitTests(argc, argv);
    int rc2 = tsunit::runUnitTests(argc, argv);

    return rc2;
}
