/* ==========================================================================
 * @(#)File: UT_TSUnit.cpp
 * Created: 2022-02-01
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

TSUNIT_TEST(Standardtests, allMacros)
{
    UT_EXPECT_EQ(3, 3);
    UT_EXPECT_NE(3, 4);
    UT_EXPECT_TRUE(true);
    UT_EXPECT_FALSE(false);
}

#include <string>

class FixtureTests : public tsunit::Test
{
public:
    static std::string fixtureCallOrder;

    void setCustomPrefix(const std::string& inCustomString)
    {
        m_customStringPrefix = inCustomString;
    }

    FixtureTests() {
        fixtureCallOrder.append(m_customStringPrefix);
        fixtureCallOrder.append(1, 'C');
    }

    ~FixtureTests() {
        fixtureCallOrder.append(m_customStringPrefix);
        fixtureCallOrder.append(1, 'D');
    }

    void SetUp()
    {
        fixtureCallOrder.append(m_customStringPrefix);
        fixtureCallOrder.append(1, 'S');
    }

    void TearDown()
    {
        fixtureCallOrder.append(m_customStringPrefix);
        fixtureCallOrder.append(1, 'T');
    }

private:
    std::string m_customStringPrefix;
}; // class FixtureTests : public tsunit::Test


std::string FixtureTests::fixtureCallOrder;

TSUNIT_TESTF(FixtureTests, firstCall)
{
}

TSUNIT_TESTF(FixtureTests, secondCall)
{
    setCustomPrefix("Holla!");
}

TSUNIT_TESTF(FixtureTests, thirdCall)
{
}

TSUNIT_TESTF(FixtureTests, fourthCall)
{
}

TSUNIT_TESTF(FixtureTests, fifthCall)
{
}

TSUNIT_TEST(FixtureTests, checkcalls)
{
    UT_EXPECT_EQ("CSTDCSHolla!THolla!DCSTDCSTDCSTD", FixtureTests::fixtureCallOrder);
}
