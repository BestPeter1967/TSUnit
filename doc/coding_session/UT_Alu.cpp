// UT_Alu.cpp
#include "TSUnit.hpp" // this is the header of our TSUnit Test
#include "alu.h" // This is the Header for the Code under Test
TSUNIT_TEST(ALU_BasicTests, checkIfAddWorks)
{
    UT_EXPECT_EQ(alu_add(3, 4), 3 + 4);
    UT_EXPECT_EQ(alu_sub(3, 4), 3 - 4);
}
