//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/harness/tests/tstSoftEqual.cc
 * \brief  Tests for class SoftEqual.
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//

#include "../SoftEqual.hh"

#include "harness/Testing.hh"

#include <list>
#include <vector>

//---------------------------------------------------------------------------//
// TESTS
//---------------------------------------------------------------------------//

TEST(SoftEqualTest, values)
{
    // Test construction
    yayp::SoftEqual<double, float> se1;
    EXPECT_EQ(1.0e-14, se1.abs_tol());
    EXPECT_EQ(1.0e-12, se1.rel_tol());

    yayp::SoftEqual<double, double> se2(0.5, 0.4);
    EXPECT_EQ(0.5, se2.rel_tol());
    EXPECT_EQ(0.4, se2.abs_tol());

    // Basic comparisons with default precisions
    EXPECT_TRUE(yayp::softEqual(1.0, 1.0));
    EXPECT_TRUE(yayp::softEqual(1.0, 1.0f));
    EXPECT_FALSE(yayp::softEqual(1.0, 0.0));
    EXPECT_FALSE(yayp::softEqual(1.0f, 0.0));

    // Test with tolerance
    yayp::SoftEqual<double, double> se3;
    EXPECT_TRUE(se3(1, 1 + se3.rel_tol() / 2));
    EXPECT_FALSE(se3(1, 1 + se3.rel_tol() * 2));

    // Basic comparisons with looser relative precision
    EXPECT_TRUE(yayp::softEqual(1.0, 1.01, 0.1));
    EXPECT_FALSE(yayp::softEqual(1.0, 1.11, 0.1));
    EXPECT_TRUE(yayp::softEqual(1.0, 1.01f, 0.1));
    EXPECT_FALSE(yayp::softEqual(1.0, 1.11f, 0.1));

    // Comparisons with looser absolute and relative precision
    EXPECT_FALSE(yayp::softEqual(1.0, 1.01, 0.001, 0.1));
    EXPECT_TRUE(yayp::softEqual(1.0, 1.0001, 0.1, 0.001));

    // Test signed zeros
    EXPECT_FALSE((yayp::softEqual<double, double>(-0, 1)));
    EXPECT_FALSE((yayp::softEqual<double, double>(1, -0)));
    EXPECT_TRUE((yayp::softEqual<double, double>(0, -0)));
    EXPECT_TRUE((yayp::softEqual<double, double>(-0, 0)));

    // Test NaNs
    const double nan = std::numeric_limits<double>::quiet_NaN();
    EXPECT_FALSE((yayp::softEqual<double, double>(1, nan)));
    EXPECT_FALSE((yayp::softEqual<double, double>(nan, 1)));
    EXPECT_FALSE(yayp::softEqual(nan, nan));

    // Test infinities
    const double inf    = std::numeric_limits<double>::infinity();
    const double maxval = std::numeric_limits<double>::max();
    EXPECT_FALSE((yayp::softEqual<double, double>(0, inf)));
    EXPECT_FALSE((yayp::softEqual<double, double>(inf, 0)));
    EXPECT_TRUE(yayp::softEqual(inf, inf));
    EXPECT_FALSE(yayp::softEqual(inf, -inf));
    EXPECT_FALSE(yayp::softEqual(-inf, inf));
    EXPECT_FALSE(yayp::softEqual(inf, maxval));
}

//---------------------------------------------------------------------------//
TEST(SoftEqualTest, containers)
{
    // Test soft-equality of two containers
    std::vector<double> vd1 = {1.0, 2.0, 3.0, 4.0};
    std::list<float>    lf  = {1.0f, 2.0f, 3.0f, 4.0f};
    std::vector<double> vd2 = {1.0, 2.0, 3.0};
    std::vector<double> vd3 = {1.0, 2.0, 3.1};

    // Run checks
    EXPECT_TRUE(yayp::softContainerEqual(vd1, vd1));
    EXPECT_TRUE(yayp::softContainerEqual(lf, lf));
    EXPECT_TRUE(yayp::softContainerEqual(vd1, lf));
    EXPECT_FALSE(yayp::softContainerEqual(vd1, vd2));
    EXPECT_FALSE(yayp::softContainerEqual(vd1, vd3));
}

//---------------------------------------------------------------------------//
// end of src/harness/tests/tstSoftEqual.cc
//---------------------------------------------------------------------------//
