//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/harness/Testing.hh
 * \brief  Function and macro declarations to support unit testing
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//
#ifndef YAYP_HARNESS_TESTING_HH
#define YAYP_HARNESS_TESTING_HH

#include <gtest/gtest.h>

namespace testing_detail
{
// Return whether two values are approximately equal
template<typename T1, typename T2>
::testing::AssertionResult isSoftEqual(const char* expected_expr,
                                       const char* actual_expr,
                                       T1          expected_val,
                                       T2          actual_val);

// Return whether two values are approximately equal within the given relative
// precision
template<typename T1, typename T2, typename Tolerance>
::testing::AssertionResult isSoftEqual(const char* expected_expr,
                                       const char* actual_expr,
                                       const char*,
                                       T1        expected_val,
                                       T2        actual_val,
                                       Tolerance rel_tol);

// Return whether two containers are approximately equal
template<class Container1, class Container2>
::testing::AssertionResult isSoftContainerEqual(const char* expected_expr,
                                                const char* actual_expr,
                                                const Container1& expected,
                                                const Container2& actual);

// Return whether two containers are equal within the given relative precision
template<class Container1, class Container2, typename Tolerance>
::testing::AssertionResult isSoftContainerEqual(const char* expected_expr,
                                                const char* actual_expr,
                                                const char*,
                                                const Container1& expected,
                                                const Container2& actual,
                                                Tolerance         rel_tol);

// Return whether two containers are exactly equal
template<class Container1, class Container2>
::testing::AssertionResult isContainerEqual(const char*       expected_expr,
                                            const char*       actual_expr,
                                            const Container1& expected,
                                            const Container2& actual);

//---------------------------------------------------------------------------//
} // namespace testing_detail

// GTest-compliant macros for soft equality test between two floating point
// values
#define EXPECT_SOFT_EQ(expected, actual) \
    EXPECT_PRED_FORMAT2(testing_detail::isSoftEqual, expected, actual)
#define EXPECT_SOFTEQ(expected, actual, rel_tol) \
    EXPECT_PRED_FORMAT3(testing_detail::isSoftEqual, expected, actual, rel_tol)

// GTest-compliant macros for soft equality test between two containers of
// floating point values.
#define EXPECT_CONT_SOFT_EQ(expected, actual) \
    EXPECT_PRED_FORMAT2(testing_detail::isSoftContainerEqual, expected, actual)
#define EXPECT_CONT_SOFTEQ(expected, actual, rel_tol) \
    EXPECT_PRED_FORMAT3(                              \
        testing_detail::isSoftContainerEqual, expected, actual, rel_tol)

// GTest-compliant macros for equality test between two containers
#define EXPECT_CONT_EQ(expected, actual) \
    EXPECT_PRED_FORMAT2(testing_detail::isContainerEqual, expected, actual)

//---------------------------------------------------------------------------//
// INLINE FUNCTION DEFINITIONS
//---------------------------------------------------------------------------//
#include "Testing.i.hh"

//---------------------------------------------------------------------------//
#endif // YAYP_HARNESS_TESTING_HH
//---------------------------------------------------------------------------//
// end of src/harness/Testing.hh
//---------------------------------------------------------------------------//
