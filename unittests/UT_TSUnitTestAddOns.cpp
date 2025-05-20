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

    std::uint32_t cnt[32];
    memset(cnt, 0, sizeof(cnt));
    constexpr std::size_t kIterations = 1000000;
    for (unsigned int i=0; i < kIterations; ++i)
    {
        const std::uint32_t num =tsunit::pseudoRandom();
        std::uint32_t mask = 1ul;
        for (int j= 0; j<32; ++j)
        {
            if (num & mask)
            {
                ++cnt[j];
            }
            mask <<= 1;
        }
    }

    unsigned int sumCnt = 0;
    for (unsigned int i=0; i < 32; ++i)
    {
        sumCnt += cnt[i];
    }

    // Treat a deviation of +/- 1% as acceptable
    constexpr unsigned int kMaxDAcceptableDeviationPercent = 1; // The +/- Deviation in %

    const uint32_t averageCnt = sumCnt >> 5; // 32
    const unsigned int absDeviation = (kMaxDAcceptableDeviationPercent * averageCnt) / 100;
    const unsigned int maxDeviation = averageCnt + absDeviation;
    const unsigned int minDeviation = averageCnt - absDeviation;

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

TSUNIT_TEST(TestAddOns_PseudoRandom, CheckDistance)
{
    int64_t distanceSum = 0ull;

    tsunit::pseudoRandomsetSeed(2312);
    int32_t lastNum;
    constexpr std::size_t kIterations = 10000000;
    constexpr unsigned int kMaxDAcceptableDeviation = 4000; // The +/- absolute Deviation
    for (int i=0; i < kIterations; ++i)
    {
        int32_t num = tsunit::pseudoRandom();

        if (i > 0)
        {
            const int32_t deltaSum = (lastNum - num);
            distanceSum += int32_t(deltaSum);
        }

        lastNum = num;
    }
    distanceSum /= (kIterations - 1);
    if (abs(distanceSum) >= kMaxDAcceptableDeviation)
    {
        printf("distanceSum = %d\n", distanceSum);
    }
    UT_EXPECT_TRUE(abs(distanceSum) < kMaxDAcceptableDeviation);
}

TSUNIT_TEST(TestAddOns_PseudoRandom, CheckReproducebility)
{
    constexpr std::size_t kIterations = 1000000;
    std::unique_ptr<uint32_t> storedNumbers(new uint32_t[kIterations]);
    uint32_t* storedNumbersPtr = storedNumbers.get();

    tsunit::pseudoRandomsetSeed(0);
    for (unsigned int i=0; i < kIterations; ++i)
    {
        storedNumbersPtr[i] = tsunit::pseudoRandom();
    }

    // Keep the random numbers running - so expect that the sequence
    // will **not** repeat.
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
        UT_EXPECT_FALSE(allMatches);
    }

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
    std::uint32_t cnt[32];

    uint8_t testData[71];
    memset(testData, 0, sizeof(testData));

    tsunit::CHasher hash;

    memset(cnt, 0, sizeof(cnt));
    constexpr std::size_t kIterations = 1000000;
    for (unsigned int i=0; i < kIterations; ++i)
    {
        hash.add(testData, sizeof(testData));
        std::uint32_t num = hash.value();
        int j=0;

        std::uint32_t mask = 1ul;
        for (int j= 0; j<32; ++j)
        {
            if (num & mask)
            {
                ++cnt[j];
            }
            mask <<= 1;
        }
    }

    unsigned int sumCnt = 0;
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
        if (!success)
        {
            UT_EXPECT_TRUE(success);
            break;
        }
    }
}

TSUNIT_TEST(TestAddOns_Hash, CheckEntropy)
{
    std::uint32_t cnt[32];

    const char testData[] = "Das Reh springt hoch, das Reh springt weit, das darf es auch, es hat ja Zeit...";

    tsunit::CHasher hash;

    memset(cnt, 0, sizeof(cnt));
    constexpr std::size_t kIterations = 10000;
    for (unsigned int i=0; i < kIterations; ++i)
    {
        hash.add(testData, sizeof(testData));
        std::uint32_t num = hash.value();

        std::uint32_t mask = 1ul;
        for (int j= 0; j<32; ++j)
        {
            if (num & mask)
            {
                ++cnt[j];
            }
            mask <<= 1;
        }
    }

    unsigned int sumCnt = 0;
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
