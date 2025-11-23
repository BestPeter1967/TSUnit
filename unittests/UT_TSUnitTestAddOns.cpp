/* ==========================================================================
 * @(#)File: UT_TSUnitTestAddOns.cpp
 * Created: 2023-10-22
 * --------------------------------------------------------------------------
 *  (c)1982-2023 Tangerine-Software
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
#include "TSUnitTestAddOns.hpp"
#include "TSUnit.hpp"
#include <cstring>
#include <cmath>

TSUNIT_TEST(TestAddOns, ROTL)
{
    UT_EXPECT_EQ(tsunit::rotl<0>(0b11010100101010001010100100101010)
                               , 0b11010100101010001010100100101010);

    UT_EXPECT_EQ(tsunit::rotl<32>(0b11010100101010001010100100101010)
                                , 0b11010100101010001010100100101010);

    UT_EXPECT_EQ(tsunit::rotl<5>(0b11010100101010001010100100101010)
                               , 0b10010101000101010010010101011010);

    UT_EXPECT_EQ(tsunit::rotl<32 + 5>(0b11010100101010001010100100101010)
                                    , 0b10010101000101010010010101011010);
}

TSUNIT_TEST(TestAddOns, ROTR)
{
    UT_EXPECT_EQ(tsunit::rotr<0>(0b11010100101010001010100100101010)
                               , 0b11010100101010001010100100101010);

    UT_EXPECT_EQ(tsunit::rotr<32>(0b11010100101010001010100100101010)
                                , 0b11010100101010001010100100101010);

    UT_EXPECT_EQ(tsunit::rotr<5>(0b11010100101010001010100100101010)
                               , 0b01010110101001010100010101001001);

    UT_EXPECT_EQ(tsunit::rotr<32+5>(0b11010100101010001010100100101010)
                                  , 0b01010110101001010100010101001001);
}

TSUNIT_TEST(TestAddOns_PseudoRandom, CheckEntropy)
{
    tsunit::pseudoRandomsetSeed(2312);

    std::uint32_t cnt[32] = {0};

    constexpr std::size_t kIterations = 10000;
    for (unsigned int i=0; i < kIterations; ++i)
    {
        const std::uint32_t num =tsunit::pseudoRandom();
        // Loop throught the 32 bits and sum up all bits set.
        for (unsigned int j= 0; j<32; ++j)
        {
            if (num & (1ul<<j))
            {
                ++cnt[j];
            }
        }
    }

    unsigned long sumCnt = 0;
    // Loop throught the 32 bits and sum up all bits set in total.
    for (unsigned int i=0; i < 32; ++i)
    {
        sumCnt += cnt[i];
    }

    // Treat a deviation of +/- 4% as acceptable
    constexpr unsigned int kMaxDAcceptableDeviationPercent = 4; // The +/- Deviation in %

    const uint32_t averageCnt = sumCnt >> 5; // 32
    const unsigned int absDeviation = (kMaxDAcceptableDeviationPercent * averageCnt) / 100;
    const unsigned int maxDeviation = averageCnt + absDeviation;
    const unsigned int minDeviation = averageCnt - absDeviation;

    // Loop throught the 32 bits...
    for (unsigned int i=0; i < 32; ++i)
    {
        const bool success((cnt[i] >= minDeviation) && (cnt[i] <= maxDeviation));
        if (!success)
        {
            UT_EXPECT_TRUE(success);
            break;
        }
    }
}

TSUNIT_TEST(TestAddOns_PseudoRandom, CheckReproducebility)
{
    constexpr std::size_t kIterations = 10000;
    std::unique_ptr<std::uint32_t> storedNumbers(new std::uint32_t[kIterations]);
    std::uint32_t* storedNumbersPtr = storedNumbers.get();

    tsunit::pseudoRandomsetSeed(0);
    for (unsigned int i=0; i < kIterations; ++i)
    {
        storedNumbersPtr[i] = tsunit::pseudoRandom();
    }

    bool allMatches = true;
    for (unsigned int i=0; i < kIterations; ++i)
    {
        if (storedNumbersPtr[i] != tsunit::pseudoRandom())
        {
            allMatches = false;
            break;
        }
    }
    UT_EXPECT_FALSE(allMatches);

    // Now, reset the seed to that which has been used to create the sequence
    // and hence expect that the sequence repeats
    tsunit::pseudoRandomsetSeed(0);
    {
        bool allMatches = true;
        for (unsigned int i=0; i < kIterations; ++i)
        {
            if (storedNumbersPtr[i] != tsunit::pseudoRandom())
            {
                allMatches = false;
                break;
            }
        }
        UT_EXPECT_TRUE(allMatches);
    }
}

TSUNIT_TEST(TestAddOns_Hash, CheckEntropyWithZeroInput)
{
    std::uint32_t cnt[32]= {0};

    std::uint8_t testData[71] = {0};
    tsunit::CHasher hash;

    constexpr std::size_t kIterations = 10000;
    for (unsigned int i=0; i < kIterations; ++i)
    {
        hash.add(testData, sizeof(testData));
        std::uint32_t num = hash.value();
        int j=0;

        for (unsigned int j= 0; j<32; ++j)
        {
            cnt[j] += (0 != (num & (1ul << j)));
        }
    }

    unsigned int sumCnt = 0;

    // Loop throught the 32 bits and sum up the total cnt
    for (unsigned int i=0; i < 32; ++i)
    {
        sumCnt += cnt[i];
    }

    // Treat a deviation of +/- 3% as acceptable
    constexpr unsigned int kMaxDAcceptableDeviationPercent = 3; // The +/- Deviation in %

    const uint32_t averageCnt = sumCnt >> 5; // 32
    const unsigned int absDeviation = (kMaxDAcceptableDeviationPercent * averageCnt) / 100;

    const unsigned int maxDeviation = averageCnt + absDeviation;
    const unsigned int minDeviation = averageCnt - absDeviation;

    // Loop through the counted 32 bits and check if their count values
    // are within the expected tolerance.
    for (unsigned int i=0; i < 32; ++i)
    {
        const bool success((cnt[i] >= minDeviation) && (cnt[i] <= maxDeviation));
        if (!success)
        {
            UT_EXPECT_TRUE(success);
            break;
        }
    }
}

TSUNIT_TEST(TestAddOns_Hash, CheckEntropy)
{
    std::uint32_t cnt[32] = {0};

    const char testData[] = "Das Reh springt hoch, das Reh springt weit, das darf es auch, es hat ja Zeit...";

    tsunit::CHasher hash;
    constexpr std::size_t kIterations = 10000;
    for (unsigned int i=0; i < kIterations; ++i)
    {
        hash.add(testData, sizeof(testData));
        std::uint32_t num = hash.value();

        for (unsigned int j= 0; j<32; ++j)
        {
            cnt[j] += (0 != (num & (1ul << j)));
        }
    }

    unsigned int sumCnt = 0;
    // Loop throught the 32 bits and sum up the total cnt
    for (unsigned int i=0; i < 32; ++i)
    {
        sumCnt += cnt[i];
    }

    // Treat a deviation of +/- 3% as acceptable
    constexpr unsigned int kMaxDAcceptableDeviationPercent = 3; // The +/- Deviation in %

    const uint32_t averageCnt = sumCnt >> 5; // 32
    const unsigned int absDeviation = (kMaxDAcceptableDeviationPercent * averageCnt) / 100;

    const unsigned int maxDeviation = averageCnt + absDeviation;
    const unsigned int minDeviation = averageCnt - absDeviation;

    for (unsigned int i=0; i < 32; ++i)
    {
        const bool success((cnt[i] >= minDeviation) && (cnt[i] <= maxDeviation));
        UT_EXPECT_TRUE(success);
    }
}
