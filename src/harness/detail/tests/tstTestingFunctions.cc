//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/harness/detail/tests/tstTestingFunctions.cc
 * \brief  Tests for class TestingFunctions.
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle,
 * LLC.
 */
//---------------------------------------------------------------------------//

#include "harness/detail/TestingFunctions.hh"

#include <gtest/gtest.h>

#include <list>
#include <vector>

//---------------------------------------------------------------------------//
// TESTS
//---------------------------------------------------------------------------//

TEST(TestingFunctionsTest, calc_rel_diffs)
{
    // Create two floating-point containers
    std::vector<double> vd = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::list<float>    lf = {1.0f, 2.1f, 3.0f, 4.2f, 5.3f};

    auto result = yayp::detail::calc_rel_diffs({1, 3, 4}, vd, lf);
    std::vector<double> ref
        = {(2.1 - 2.0) / 2.1, (4.2 - 4.0) / 4.2, (5.3 - 5.0) / 5.3};

    EXPECT_NEAR(ref[0], result[0], 1.0e-6);
    EXPECT_NEAR(ref[1], result[1], 1.0e-6);
    EXPECT_NEAR(ref[2], result[2], 1.0e-6);
}

//---------------------------------------------------------------------------//

TEST(TestingFunctionsTest, max_field_width)
{
    // Create a container of doubles
    std::vector<double>       v         = {1.0, 2.0, 3.0, 4.555, 5.2};
    unsigned int              precision = 16;
    std::string               header    = "Dbl";
    std::vector<unsigned int> indices   = {0, 1, 2, 3, 4};
    std::size_t               result
        = yayp::detail::find_max_field_width(header, indices, v, precision);
    EXPECT_EQ(6, result);

    // Create a container of strings
    std::vector<std::string> s = {"str1", "str2", "longer"};
    header                     = "Str";
    indices                    = {0, 1, 2};
    result = yayp::detail::find_max_field_width(header, indices, s, precision);
    EXPECT_EQ(7, result);
}

//---------------------------------------------------------------------------//

TEST(TestingFunctionsTest, write_unequal_values)
{
    // Create a couple of floating point containers
    std::vector<double>       vd      = {0.0, 1.0, 2.0, 3.0, 4.0};
    std::list<double>         ld      = {0.1, 1.0, 2.0, 3.2, 4.3};
    std::vector<unsigned int> indices = {0, 3, 4};
    std::string               result
        = yayp::detail::write_unequal_values(indices, "vd", "ld", vd, ld);
    std::string ref = R"( Index vd  ld          Rel. Diff.
     0  0 0.1                   1
     3  3 3.2 0.06250000000000006
     4  4 4.3 0.06976744186046507)";
    EXPECT_EQ(ref, result);

    // Create some string containers
    std::vector<std::string> s1 = {"str1 ", " str2 ", " str3 ", " str4 "};
    std::vector<std::string> s2 = {"str1", "strb", "strc", "str4"};
    indices                     = {1, 2};
    result = yayp::detail::write_unequal_values(indices, "s1", "s2", s1, s2);
    ref    = R"( Index     s1   s2
     1  str2  strb
     2  str3  strc)";
    EXPECT_EQ(ref, result);
}

//---------------------------------------------------------------------------//
// end of src/harness/detail/tests/tstTestingFunctions.cc
//---------------------------------------------------------------------------//
