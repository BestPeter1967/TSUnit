/* ==========================================================================
 * @(#)File: UT_CPoorMansBlockAlloc.cpp
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
#include "CPoorMansBlockAlloc.hpp"
#include <unordered_set>

static const uint8_t kTestData[] = {
    0x67, 0xc6, 0x69, 0x73, 0x51, 0xff, 0x4a, 0xec,
    0x29, 0xcd, 0xba, 0xab, 0xf2, 0xfb, 0xe3, 0x46,
    0x7c, 0xc2, 0x54, 0xf8, 0x1b, 0xe8, 0xe7, 0x8d,
    0x76, 0x5a, 0x2e, 0x63, 0x33, 0x9f, 0xc9, 0x9a,
    0x66, 0x32, 0x0d, 0xb7, 0x31, 0x58, 0xa3, 0x5a,
    0x25, 0x5d, 0x05, 0x17, 0x58, 0xe9, 0x5e, 0xd4,
    0xab, 0xb2, 0xcd, 0xc6, 0x9b, 0xb4, 0x54, 0x11,
    0x0e, 0x82, 0x74, 0x41, 0x21, 0x3d, 0xdc, 0x87,
    0x70, 0xe9, 0x3e, 0xa1, 0x41, 0xe1, 0xfc, 0x67,
    0x3e, 0x01, 0x7e, 0x97, 0xea, 0xdc, 0x6b, 0x96,
    0x8f, 0x38, 0x5c, 0x2a, 0xec, 0xb0, 0x3b, 0xfb,
    0x32, 0xaf, 0x3c, 0x54, 0xec, 0x18, 0xdb, 0x5c,
    0x02, 0x1a, 0xfe, 0x43, 0xfb, 0xfa, 0xaa, 0x3a,
    0xfb, 0x29, 0xd1, 0xe6, 0x05, 0x3c, 0x7c, 0x94,
    0x75, 0xd8, 0xbe, 0x61, 0x89, 0xf9, 0x5c, 0xbb,
    0xa8, 0x99, 0x0f, 0x95, 0xb1, 0xeb, 0xf1, 0xb3,
    0x05, 0xef, 0xf7, 0x00, 0xe9, 0xa1, 0x3a, 0xe5,
    0xca, 0x0b, 0xcb, 0xd0, 0x48, 0x47, 0x64, 0xbd,
    0x1f, 0x23, 0x1e, 0xa8, 0x1c, 0x7b, 0x64, 0xc5,
    0x14, 0x73, 0x5a, 0xc5, 0x5e, 0x4b, 0x79, 0x63,
    0x3b, 0x70, 0x64, 0x24, 0x11, 0x9e, 0x09, 0xdc,
    0xaa, 0xd4, 0xac, 0xf2, 0x1b, 0x10, 0xaf, 0x3b,
    0x33, 0xcd, 0xe3, 0x50, 0x48, 0x47, 0x15, 0x5c,
    0xbb, 0x6f, 0x22, 0x19, 0xba, 0x9b, 0x7d, 0xf5,
    0x0b, 0xe1, 0x1a, 0x1c, 0x7f, 0x23, 0xf8, 0x29,
    0xf8, 0xa4, 0x1b, 0x13, 0xb5, 0xca, 0x4e, 0xe8,
    0x98, 0x32, 0x38, 0xe0, 0x79, 0x4d, 0x3d, 0x34,
    0xbc, 0x5f, 0x4e, 0x77, 0xfa, 0xcb, 0x6c, 0x05,
    0xac, 0x86, 0x21, 0x2b, 0xaa, 0x1a, 0x55, 0xa2,
    0xbe, 0x70, 0xb5, 0x73, 0x3b, 0x04, 0x5c, 0xd3,
    0x36, 0x94, 0xb3, 0xaf, 0xe2, 0xf0, 0xe4, 0x9e,
    0x4f, 0x32, 0x15, 0x49, 0xfd, 0x82, 0x4e, 0xa9
};

TSUNIT_TEST(Basictests, checkIfInitiallyEmpty)
{
    using ThisType = uint8_t[8];
    CPoorMansBlockAlloc<ThisType, 3> allocator;

    UT_EXPECT_EQ(0, allocator.used());
    UT_EXPECT_EQ(8, allocator.available());
    UT_EXPECT_TRUE(allocator.empty());
    UT_EXPECT_FALSE(allocator.full());
}

TSUNIT_TEST(Basictests, allocateOneAndCheckIfOccupied)
{
    using ThisType = uint8_t[8];
    CPoorMansBlockAlloc<ThisType, 3> allocator;

    allocator.alloc();
    UT_EXPECT_EQ(1, allocator.used());
    UT_EXPECT_EQ(7, allocator.available());
    UT_EXPECT_FALSE(allocator.empty());
    UT_EXPECT_FALSE(allocator.full());

}

TSUNIT_TEST(Basictests, allocateAllAfterAnotherAndCheckIfOccupied)
{
    using ThisType = uint8_t[8];
    CPoorMansBlockAlloc<ThisType, 3> allocator;

    // First alloc
    allocator.alloc();
    UT_EXPECT_EQ(1, allocator.used());
    UT_EXPECT_EQ(7, allocator.available());
    UT_EXPECT_FALSE(allocator.empty());
    UT_EXPECT_FALSE(allocator.full());

    // Second alloc
    allocator.alloc();
    UT_EXPECT_EQ(3 /*2*/, allocator.used());
    UT_EXPECT_EQ(6, allocator.available());
    UT_EXPECT_FALSE(allocator.empty());
    UT_EXPECT_FALSE(allocator.full());

    // Third alloc
    allocator.alloc();
    UT_EXPECT_EQ(3, allocator.used());
    UT_EXPECT_EQ(5, allocator.available());
    UT_EXPECT_FALSE(allocator.empty());
    UT_EXPECT_FALSE(allocator.full());

    // Forth alloc
    allocator.alloc();
    UT_EXPECT_EQ(4, allocator.used());
    UT_EXPECT_EQ(4, allocator.available());
    UT_EXPECT_FALSE(allocator.empty());
    UT_EXPECT_FALSE(allocator.full());

    // Fifth alloc
    allocator.alloc();
    UT_EXPECT_EQ(5, allocator.used());
    UT_EXPECT_EQ(3, allocator.available());
    UT_EXPECT_FALSE(allocator.empty());
    UT_EXPECT_FALSE(allocator.full());

    // Sixth alloc
    allocator.alloc();
    UT_EXPECT_EQ(6, allocator.used());
    UT_EXPECT_EQ(2, allocator.available());
    UT_EXPECT_FALSE(allocator.empty());
    UT_EXPECT_FALSE(allocator.full());

    // Seventh alloc
    allocator.alloc();
    UT_EXPECT_EQ(7, allocator.used());
    UT_EXPECT_EQ(1, allocator.available());
    UT_EXPECT_FALSE(allocator.empty());
    UT_EXPECT_FALSE(allocator.full());

    // Eight alloc
    allocator.alloc();
    UT_EXPECT_EQ(8, allocator.used());
    UT_EXPECT_EQ(0, allocator.available());
    UT_EXPECT_FALSE(allocator.empty());
    UT_EXPECT_TRUE(allocator.full());

    // Nineth alloc (expect ot fail)
    allocator.alloc();
    UT_EXPECT_EQ(8, allocator.used());
    UT_EXPECT_EQ(0, allocator.available());
    UT_EXPECT_FALSE(allocator.empty());
    UT_EXPECT_TRUE(allocator.full());
}

TSUNIT_TEST(Basictests, checkIfClearFreesAllMemory)
{
    using ThisType = uint8_t[8];
    CPoorMansBlockAlloc<ThisType, 3> allocator;

    // First, allocate 8 slots
    allocator.alloc();
    allocator.alloc();
    allocator.alloc();
    allocator.alloc();
    allocator.alloc();
    allocator.alloc();
    allocator.alloc();
    allocator.alloc();

    allocator.clear();

    UT_EXPECT_EQ(0, allocator.used());
    UT_EXPECT_EQ(8, allocator.available());
    UT_EXPECT_TRUE(allocator.empty());
    UT_EXPECT_FALSE(allocator.full());

}

TSUNIT_TEST(Basictests, checkIfAllocationBeyondBoundariesWillreturnANullptr)
{
    using ThisType = uint8_t[8];
    CPoorMansBlockAlloc<ThisType, 3> allocator;

    ThisType* returnPtr;

    returnPtr = allocator.alloc();
    UT_EXPECT_NE(nullptr, returnPtr );

    returnPtr = allocator.alloc();
    UT_EXPECT_NE(nullptr, returnPtr );

    returnPtr = allocator.alloc();
    UT_EXPECT_NE(nullptr, returnPtr );

    returnPtr = allocator.alloc();
    UT_EXPECT_NE(nullptr, returnPtr );

    returnPtr = allocator.alloc();
    UT_EXPECT_NE(nullptr, returnPtr );

    returnPtr = allocator.alloc();
    UT_EXPECT_NE(nullptr, returnPtr );

    returnPtr = allocator.alloc();
    UT_EXPECT_NE(nullptr, returnPtr );

    returnPtr = allocator.alloc();
    UT_EXPECT_NE(nullptr, returnPtr );

    // Another one should return a nullptr!
    returnPtr = allocator.alloc();
    UT_EXPECT_EQ(nullptr, returnPtr );
}

TSUNIT_TEST(Basictests, checkIfWeGetEightDifferentPointers)
{
    using ThisType = uint8_t[8];
    CPoorMansBlockAlloc<ThisType, 3> allocator;

    std::unordered_set<ThisType*> allPointers;

    allPointers.insert(allocator.alloc());
    allPointers.insert(allocator.alloc());
    allPointers.insert(allocator.alloc());
    allPointers.insert(allocator.alloc());
    allPointers.insert(allocator.alloc());
    allPointers.insert(allocator.alloc());
    allPointers.insert(allocator.alloc());
    allPointers.insert(allocator.alloc());

    UT_EXPECT_EQ(8, allPointers.size());
}

TSUNIT_TEST(Basictests, checkIfAllFAllocatedPointerWillBeRegainedUponFree)
{
    using ThisType = uint8_t[8];
    CPoorMansBlockAlloc<ThisType, 3> allocator;

    ThisType* returnPtr[8];

    returnPtr[0]  = allocator.alloc();
    returnPtr[1]  = allocator.alloc();
    returnPtr[2]  = allocator.alloc();
    returnPtr[3]  = allocator.alloc();
    returnPtr[4]  = allocator.alloc();
    returnPtr[5]  = allocator.alloc();
    returnPtr[6]  = allocator.alloc();
    returnPtr[7]  = allocator.alloc();

    allocator.free(returnPtr[0]);
    UT_EXPECT_EQ(7, allocator.used());
    UT_EXPECT_EQ(1, allocator.available());
    UT_EXPECT_FALSE(allocator.empty());
    UT_EXPECT_FALSE(allocator.full());
}

TSUNIT_TEST(Basictests, checkIfAttemptingToFreeANullptrWillSucceeed)
{
    using ThisType = uint8_t[8];
    CPoorMansBlockAlloc<ThisType, 3> allocator;

    ThisType* returnPtr[3];

    UT_EXPECT_TRUE(allocator.free(nullptr));
}

TSUNIT_TEST(Basictests, checkIfAttemptingToFreeMorePointersThanAllocatedWillFail)
{
    using ThisType = uint8_t[8];
    CPoorMansBlockAlloc<ThisType, 3> allocator;

    ThisType* returnPtr[3];

    returnPtr[0]  = allocator.alloc();
    returnPtr[1]  = allocator.alloc();
    returnPtr[2]  = allocator.alloc();

    UT_EXPECT_TRUE(allocator.free(returnPtr[0]));
    UT_EXPECT_TRUE(allocator.free(returnPtr[1]));
    UT_EXPECT_TRUE(allocator.free(returnPtr[2]));

    UT_EXPECT_FALSE(allocator.free(returnPtr[2]));
}

class TestFixture1 : public tsunit::Test
{
    template <typename T>
    class CTestData {
    public:
        CTestData(){}

        void fill(T* outData)
        {
            for (int i = 0; i < sizeof(T); ++i)
            {
                (*outData)[i] = kTestData[((i+4)*7) & 0xff];
            }
        }

        bool verify(const T* inData) const
        {
            bool match = true;
            for (int i = 0; i < sizeof(T); ++i)
            {
                if ((*inData)[i] != kTestData[((i+4)*7) & 0xff])
                {
                    match = false;
                    break;
                }
            }
            return match;
        }
    };
public:
    using ThisType = uint8_t[8];

    CPoorMansBlockAlloc<ThisType, 3> allocator;

    CTestData<ThisType> testData[8];
    ThisType* returnPtr[8];

    TestFixture1() = default;
    ~TestFixture1() = default;

    void SetUp()
    {
        for (unsigned int i=0; i < sizeof(returnPtr) / sizeof(*returnPtr); ++i)
        {
            returnPtr[i]  = allocator.alloc();
            testData[i].fill(returnPtr[i]);
        }
    }

    void TearDown() {}

    void free(size_t inIndex)
    {
        allocator.free(returnPtr[inIndex]);
        returnPtr[inIndex] = nullptr;
    }
};

TSUNIT_TESTF(TestFixture1, checkIfAllocatingANewPointerWillNotAffectAlreadyAllocatedStores)
{
    // Verify the data
    UT_EXPECT_TRUE(testData[0].verify(returnPtr[0]));
    UT_EXPECT_TRUE(testData[1].verify(returnPtr[1]));
    UT_EXPECT_TRUE(testData[2].verify(returnPtr[2]));
    UT_EXPECT_TRUE(testData[3].verify(returnPtr[3]));
    UT_EXPECT_TRUE(testData[4].verify(returnPtr[4]));
    UT_EXPECT_TRUE(testData[5].verify(returnPtr[5]));
    UT_EXPECT_TRUE(testData[6].verify(returnPtr[6]));
    UT_EXPECT_TRUE(testData[7].verify(returnPtr[7]));
}

TSUNIT_TESTF(TestFixture1, checkIfFreeingAPointerWillNotAffectAlreadyAllocatedStores)
{
    // Verify the data
    // --------------------------------------------
    // Step 1: Free Slot 2
    // --------------------------------------------
    free(2);
    UT_EXPECT_TRUE(testData[0].verify(returnPtr[0]));
    UT_EXPECT_TRUE(testData[1].verify(returnPtr[1]));
//    UT_EXPECT_TRUE(testData[2].verify(returnPtr[2]));
    UT_EXPECT_TRUE(testData[3].verify(returnPtr[3]));
    UT_EXPECT_TRUE(testData[4].verify(returnPtr[4]));
    UT_EXPECT_TRUE(testData[5].verify(returnPtr[5]));
    UT_EXPECT_TRUE(testData[6].verify(returnPtr[6]));
    UT_EXPECT_TRUE(testData[7].verify(returnPtr[7]));

    // --------------------------------------------
    // Step 2: Free Slot 0
    // --------------------------------------------
    free(0);
//  UT_EXPECT_TRUE(testData[0].verify(returnPtr[0]));
    UT_EXPECT_TRUE(testData[1].verify(returnPtr[1]));
//  UT_EXPECT_TRUE(testData[2].verify(returnPtr[2]));
    UT_EXPECT_TRUE(testData[3].verify(returnPtr[3]));
    UT_EXPECT_TRUE(testData[4].verify(returnPtr[4]));
    UT_EXPECT_TRUE(testData[5].verify(returnPtr[5]));
    UT_EXPECT_TRUE(testData[6].verify(returnPtr[6]));
    UT_EXPECT_TRUE(testData[7].verify(returnPtr[7]));

    // --------------------------------------------
    // Step 3: Free Slot 4
    // --------------------------------------------
    free(4);
//  UT_EXPECT_TRUE(testData[0].verify(returnPtr[0]));
    UT_EXPECT_TRUE(testData[1].verify(returnPtr[1]));
//  UT_EXPECT_TRUE(testData[2].verify(returnPtr[2]));
    UT_EXPECT_TRUE(testData[3].verify(returnPtr[3]));
//  UT_EXPECT_TRUE(testData[4].verify(returnPtr[4]));
    UT_EXPECT_TRUE(testData[5].verify(returnPtr[5]));
    UT_EXPECT_TRUE(testData[6].verify(returnPtr[6]));
    UT_EXPECT_TRUE(testData[7].verify(returnPtr[7]));

    // --------------------------------------------
    // Step 4: Free Slot 5
    // --------------------------------------------
    free(5);
//  UT_EXPECT_TRUE(testData[0].verify(returnPtr[0]));
    UT_EXPECT_TRUE(testData[1].verify(returnPtr[1]));
//  UT_EXPECT_TRUE(testData[2].verify(returnPtr[2]));
    UT_EXPECT_TRUE(testData[3].verify(returnPtr[3]));
//  UT_EXPECT_TRUE(testData[4].verify(returnPtr[4]));
//  UT_EXPECT_TRUE(testData[5].verify(returnPtr[5]));
    UT_EXPECT_TRUE(testData[6].verify(returnPtr[6]));
    UT_EXPECT_TRUE(testData[7].verify(returnPtr[7]));

    // --------------------------------------------
    // Step 5: Free Slot 7
    // --------------------------------------------
    free(7);
//  UT_EXPECT_TRUE(testData[0].verify(returnPtr[0]));
    UT_EXPECT_TRUE(testData[1].verify(returnPtr[1]));
//  UT_EXPECT_TRUE(testData[2].verify(returnPtr[2]));
    UT_EXPECT_TRUE(testData[3].verify(returnPtr[3]));
//  UT_EXPECT_TRUE(testData[4].verify(returnPtr[4]));
//  UT_EXPECT_TRUE(testData[5].verify(returnPtr[5]));
    UT_EXPECT_TRUE(testData[6].verify(returnPtr[6]));
//  UT_EXPECT_TRUE(testData[7].verify(returnPtr[7]));

    // --------------------------------------------
    // Step 6: Free Slot 1
    // --------------------------------------------
    free(1);
//  UT_EXPECT_TRUE(testData[0].verify(returnPtr[0]));
//  UT_EXPECT_TRUE(testData[1].verify(returnPtr[1]));
//  UT_EXPECT_TRUE(testData[2].verify(returnPtr[2]));
    UT_EXPECT_TRUE(testData[3].verify(returnPtr[3]));
//  UT_EXPECT_TRUE(testData[4].verify(returnPtr[4]));
//  UT_EXPECT_TRUE(testData[5].verify(returnPtr[5]));
    UT_EXPECT_TRUE(testData[6].verify(returnPtr[6]));
//  UT_EXPECT_TRUE(testData[7].verify(returnPtr[7]));

    // --------------------------------------------
    // Step 7: Free Slot 6
    // --------------------------------------------
    free(6);
//  UT_EXPECT_TRUE(testData[0].verify(returnPtr[0]));
//  UT_EXPECT_TRUE(testData[1].verify(returnPtr[1]));
//  UT_EXPECT_TRUE(testData[2].verify(returnPtr[2]));
    UT_EXPECT_TRUE(testData[3].verify(returnPtr[3]));
//  UT_EXPECT_TRUE(testData[4].verify(returnPtr[4]));
//  UT_EXPECT_TRUE(testData[5].verify(returnPtr[5]));
//  UT_EXPECT_TRUE(testData[6].verify(returnPtr[6]));
//  UT_EXPECT_TRUE(testData[7].verify(returnPtr[7]));
}


TSUNIT_TEST(Basictests, checkIfFreeingAnEntryMoreThanOnceBailsAnFailure)
{
    using ThisType = uint8_t[8];
    CPoorMansBlockAlloc<ThisType, 3> allocator;

    ThisType* returnPtr[4];
    returnPtr[0] = allocator.alloc();
    returnPtr[1] = allocator.alloc();
    returnPtr[2] = allocator.alloc();
    returnPtr[3] = allocator.alloc();

    UT_EXPECT_TRUE(allocator.free(returnPtr[2]));
    UT_EXPECT_FALSE(allocator.free(returnPtr[2]));

}
