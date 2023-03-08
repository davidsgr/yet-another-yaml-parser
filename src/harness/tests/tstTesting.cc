//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/harness/tests/tstTesting.cc
 * \brief  Tests for class Testing.
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//

#include "../Testing.hh"

#include <gtest/gtest.h>

#include <list>
#include <vector>

//---------------------------------------------------------------------------//
// TESTS
//---------------------------------------------------------------------------//

TEST(TestingTest, soft_value_equality)
{
    double d1 = 4.0;
    double d2 = 4.1;
    double d3 = 4.001;

    // These should pass
    EXPECT_SOFT_EQ(d1, d1);
    EXPECT_SOFT_EQ(d2, d2);

    // These should fail with an intelligible error message
    // EXPECT_SOFT_EQ(d1, d2);
    // EXPECT_SOFT_EQ(d1, d3);

    // Now, try again with different relative precision.  These should pass
    EXPECT_SOFTEQ(d1, d1, 1.0e-2);
    EXPECT_SOFTEQ(d1, d3, 1.0e-2);

    // This should fail
    // EXPECT_SOFTEQ(d1, d2, 1.0e-2);
}

//---------------------------------------------------------------------------//
TEST(TestingTest, soft_container_equality)
{
    // Create some containers
    std::vector<double> v1 = {1.0, 2.0, 3.0};
    std::vector<double> v2 = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> v3 = {1.001, 2.001, 3.001};
    std::list<double>   l1 = {1.0, 2.0, 3.0};

    // This should pass
    EXPECT_CONT_SOFT_EQ(v1, v1);
    EXPECT_CONT_SOFT_EQ(v1, l1);

    // These should fail with reasonable error messages
    // EXPECT_CONT_SOFT_EQ(v1, v2);
    // EXPECT_CONT_SOFT_EQ(v1, v3);

    // Try again with different relative precision.  These should pass
    EXPECT_CONT_SOFTEQ(v1, v1, 1.0e-2);
    EXPECT_CONT_SOFTEQ(v1, l1, 1.0e-2);
    EXPECT_CONT_SOFTEQ(v1, v3, 1.0e-2);

    // This should fail intelligibly
    // EXPECT_CONT_SOFTEQ(v1, v2, 1.0e-2);
}

//---------------------------------------------------------------------------//
TEST(TestingTest, container_equality)
{
    // Create some containers
    std::vector<int>  v1 = {1, 2, 3};
    std::vector<int>  v2 = {1, 2, 3, 4};
    std::vector<int>  v3 = {5, 6, 7};
    std::list<double> l1 = {1, 2, 3};

    // These should pass
    EXPECT_CONT_EQ(v1, v1);
    EXPECT_CONT_EQ(v1, l1);

    // These should fail intelligibly
    // EXPECT_CONT_EQ(v1, v2);
    // EXPECT_CONT_EQ(v1, v3);
}

//---------------------------------------------------------------------------//
// end of src/harness/tests/tstTesting.cc
//---------------------------------------------------------------------------//
